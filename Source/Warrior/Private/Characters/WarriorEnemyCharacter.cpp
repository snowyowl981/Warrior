// SnowyOwl All Rights Reserved


#include "Characters/WarriorEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "Components/UI/EnemyUIComponent.h"

#include "WarriorDebugHelper.h"

AWarriorEnemyCharacter::AWarriorEnemyCharacter()
{
	// AI 컨트롤러가 이 캐릭터를 자동으로 조종하도록 설정 (월드에 배치되거나 스폰될 때)
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 컨트롤러의 회전을 캐릭터에 적용하지 않음 (카메라 회전과 무관하게 캐릭터 회전)
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터 이동 설정
	// 컨트롤러가 지정한 회전을 사용하지 않음
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// 이동 방향을 기준으로 자동 회전 (예: 이동하는 방향을 바라봄)
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 회전 속도 설정 (Yaw 회전 속도 0 — 필요 시 변경 가능)
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 0.f);

	// 걷기 속도 설정
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	// 걷기 상태에서의 감속 (브레이크) 설정
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	// EnemyCombatComponent 생성 및 초기화
	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");

	// EnemyCombatComponent 생성 및 초기화
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>("EnemyUIComponent");
}

UPawnCombatComponent* AWarriorEnemyCharacter::GetCombatComponent() const
{
	// 적 전투 컴포넌트 반환
	return EnemyCombatComponent;
}

UPawnUIComponent* AWarriorEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}

// PossessedBy 함수는 이 캐릭터가 새로운 컨트롤러에 의해 조종되기 시작할 때 호출
void AWarriorEnemyCharacter::PossessedBy(AController* NewController)
{
	// 부모 클래스의 PossessedBy 함수를 호출하여 기본 처리를 수행
	Super::PossessedBy(NewController);

	// 캐릭터 초기화 데이터를 설정하는 사용자 정의 함수 호출
	InitEnemyStartUpData();
}

// 캐릭터의 시작 시 필요한 데이터를 비동기적으로 로딩하고 초기화하는 함수
void AWarriorEnemyCharacter::InitEnemyStartUpData()
{
	// CharacterStartUpData가 유효하지 않으면 (SoftObjectPtr가 null일 경우) 함수 종료
	if (CharacterStartUpData.IsNull())
	{
		return;
	}

	// AssetManager를 통해 비동기 로딩을 요청
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(), // SoftObjectPath를 통해 자산 경로 지정
		FStreamableDelegate::CreateLambda(
			[this]() // 로딩이 완료되었을 때 실행할 람다 함수 정의
			{
				// 자산이 유효하게 로딩되었는지 확인하고, Get()을 통해 실제 객체 포인터 획득
				if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
				{
					// AbilitySystemComponent에 로딩된 능력 데이터를 적용
					LoadedData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponent);
				}
			}
		)	
	);
}
