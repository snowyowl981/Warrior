// SnowyOwl All Rights Reserved


#include "Characters/WarriorHeroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/WarriorInputComponent.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Components/UI/HeroUIComponent.h"

#include "WarriorDebugHelper.h"

AWarriorHeroCharacter::AWarriorHeroCharacter()
{
	// 캡슐 컴포넌트 사이즈 설정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// 캐릭터 이동 방향에 따라 카메라 회전 X
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// 카메라 암 컴포넌트 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	// 카메라 컴포넌트 설정
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// 캐릭터 움직임 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;				// 카메라 회전에 따라 캐릭터 움직임 X
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// HeroCombatComponent 할당
	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));

	// HeroUIComponent 할당
	HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>(TEXT("HeroUIComponent"));

	
}

UPawnCombatComponent* AWarriorHeroCharacter::GetCombatComponent() const
{
	return HeroCombatComponent;
}

UPawnUIComponent* AWarriorHeroCharacter::GetPawnUIComponent() const
{
	return HeroUIComponent;
}

void AWarriorHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 소프트 참조에 유효한 데이터 자산을 할당했는지 확인
	// 이런 종류의 소프트 레퍼런스 검사엔 IsValid 대신 IsNull 사용 (IsValid는 에셋이 이미 로드됨을 의미)
	if (!CharacterStartUpData.IsNull())
	{
		// 데이터 동기식 로드 및 지역변수 할당 후 유효한지 확인
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			// 유효한 경우 로드된 데이터에 능력 시스템 컴포넌트 부여
			LoadedData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponent);
		}

	}
}

// 플레이어 입력 컴포넌트 셋업 함수
void AWarriorHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));
	// 플레이어 컨트롤러로부터 로컬 플레이어 가져와 할당
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	// 로컬 플레이어 대상으로 UEnhancedInputLocalPlayerSubsystem 할당
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	// nullptr 체크
	check(SubSystem);
	// 매핑 컨텍스트 추가 (생성해 둔 UDataAsset과 입력 매핑 컨텍스트 그룹화), 새 항목이 기존 항목보다 높아야만 오버라이드
	SubSystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
	// 플레이어 입력 컴포넌트를 UWarriorInputComponent 타입 태스팅 (null이거나 캐스팅 실패 시 크래시)
	UWarriorInputComponent* WarriorInputComponent = CastChecked<UWarriorInputComponent>(PlayerInputComponent);
	// WarriorInputComponent에 미리 생성해 둔 BindNativeInputAction 함수 사용
	// 이동 인풋 액션 바인딩
	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	// 시선 (카메라 회전) 인풋 액션 바인딩
	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	WarriorInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void AWarriorHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// 이동 함수
void AWarriorHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	// 벡터 및 회전값 설정
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	// 입력 Y좌표에 따른 전후 이동
	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	// 입력 X좌표에 따른 좌우 이동
	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection, MovementVector.X);
	}
}

// 카메라 회전 함수
void AWarriorHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	// 벡터 설정
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	
	// 시선 벡터에 따른 카메라 상하 회전
	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	// 시선 벡터에 따른 카메라 좌우 회전
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AWarriorHeroCharacter::Input_AbilityInputPressed(FGameplayTag InputTag)
{
	// 시스템 컴포넌트에 태그 전달
	WarriorAbilitySystemComponent->OnAbilityInputPressed(InputTag);
}

void AWarriorHeroCharacter::Input_AbilityInputReleased(FGameplayTag InputTag)
{
	// 시스템 컴포넌트에 태그 전달
	WarriorAbilitySystemComponent->OnAbilityInputReleased(InputTag);
}
