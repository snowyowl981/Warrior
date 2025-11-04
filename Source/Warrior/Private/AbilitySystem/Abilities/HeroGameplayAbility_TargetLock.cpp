// SnowyOwl All Rights Reserved


#include "AbilitySystem/Abilities/HeroGameplayAbility_TargetLock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/WarriorWidgetBase.h"
#include "Controllers/WarriorHeroController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"

#include "WarriorDebugHelper.h"

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 타겟 고정
	TryLockOnTarget();
	// 타겟 고정 시 이동속도 설정
	InitTargetLockMovement();
	// 타겟 고정 매핑 컨텍스트 초기화
	InitTargetLockMappingContext();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 이동 속도 재설정
	ResetTargetLockMovement();
	// 타겟 고정 매핑 컨텍스트 재설정
	ResetTargetLockMappingContext();
	// 어빌리티 종료 시 정리 로직 실행
	CleanUp();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
	// 현재 고정된 액터가 존재하지 않거나, 고정된 액터가 죽은 상태거나, 영웅 캐릭터가 죽은 상태면 타겟 고정 취소 및 함수 종료
	if (!CurrentLockedActor ||
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(CurrentLockedActor, WarriorGameplayTags::Shared_Status_Dead) ||
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Shared_Status_Dead)
		)
	{
		CancelTargetLockAbility();
		return;
	}

	// 목표 잠금 위치 지속 설정
	SetTargetLockWidgetPosition();

	// 캐릭터의 회전을 덮어쓸 수 있는지 여부를 결정하는 조건 (회피 중이거나 방어 중이 아닐 때)
	const bool bShouldOverrideRotation =
		!UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Rolling)
	&&
		!UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Blocking);

	// 회전 덮어쓰기가 가능한 상태일 때만 실행
	if (bShouldOverrideRotation)
	{
		// 캐릭터 위치에서 현재 락온된 액터를 바라보는 회전값 계산
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
			GetHeroCharacterFromActorInfo()->GetActorLocation(),    // 시작 위치 (플레이어)
			CurrentLockedActor->GetActorLocation()                  // 목표 위치 (락온된 대상)
		);

		// 현재 컨트롤러(카메라)의 회전값
		const FRotator CurrentControlRot = GetHeroControllerFromActorInfo()->GetControlRotation();

		// 현재 회전에서 목표 회전으로 부드럽게 보간 (DeltaTime 기반)
		const FRotator TargetRot = FMath::RInterpTo(
			CurrentControlRot,					// 현재 회전값
			LookAtRot,							// 목표 회전값
			DeltaTime,							// 프레임 보정(시간 기반)
			TargetLockRotationInterpSpeed		// 회전 속도 (블렌딩 정도)
		);

		// 컨트롤러(카메라) 회전 적용: Pitch, Yaw만 반영하고 Roll은 0으로 고정
		GetHeroControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));

		// 캐릭터 메쉬 자체도 동일한 방향으로 회전시킴 (수평 회전만 적용)
		GetHeroCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
	}
			
}

void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
	// 고정할 유효 액터들 배열 설정
	GetAvailableActorsToLock();

	// 액터 배열이 비었을 경우 타겟 고정 취소 및 함수 종료
	if (AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	// 현재 고정된 액터를 배열 내에서 가장 가까운 액터로 설정
	CurrentLockedActor = GetNearestTargetFromAvailableActor(AvailableActorsToLock);

	// 현재 고정된 액터가 유효한 경우 위젯 그리기, 위젯 위치 설정
	if (CurrentLockedActor)
	{
		DrawTargetLockWidget();

		SetTargetLockWidgetPosition();
	}
	// 고정된 액터가 유효하지 않은 경우 타겟 고정 취소
	else
	{
		CancelTargetLockAbility();
	}
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	// 박스 트레이스의 충돌 결과를 담을 배열 선언
	TArray<FHitResult> BoxTraceHits;
    
	// 지정된 위치와 방향으로 박스 모양의 트레이스(충돌 탐지)를 수행
	UKismetSystemLibrary::BoxTraceMultiForObjects(
	   GetHeroCharacterFromActorInfo(),														// 트레이스를 수행할 주체(보통 플레이어 캐릭터)
	   GetHeroCharacterFromActorInfo()->GetActorLocation(),									// 시작 지점
	   GetHeroCharacterFromActorInfo()->GetActorLocation() +
		   GetHeroCharacterFromActorInfo()->GetActorForwardVector() * BoxTraceDistance,		// 끝 지점 (캐릭터 전방 방향)
	   TraceBoxSize / 2.f,																	// 박스 크기 (언리얼은 반지름 형태로 받음)
	   GetHeroCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),		// 박스 회전 방향 (캐릭터 정면 방향)
	   BoxTraceChannel,																		// 충돌을 감지할 오브젝트 채널 설정
	   false,																				// 트레이스 시 자기 자신은 무시하지 않음 (아래에서 수동으로 필터링)
	   TArray<AActor*>(),																	// 무시할 액터 배열 (현재는 비어 있음)
	   bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,		// 디버그 시각화 옵션
	   BoxTraceHits,																	// 트레이스 결과를 저장할 배열 참조
	   true																					// 복잡한 충돌(Complex Trace)도 검사
	);

	// 트레이스 결과에 포함된 모든 히트 정보를 순회
	for (const FHitResult& TraceHit : BoxTraceHits)
	{
		// 히트된 액터를 가져옴
		if (AActor* HitActor = TraceHit.GetActor())
		{
			// 자기 자신은 타겟 후보에서 제외
			if (HitActor != GetHeroCharacterFromActorInfo())
			{
				// 중복 없이 액터를 목록에 추가
				AvailableActorsToLock.AddUnique(HitActor);
			}
		}
	}
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailableActor(const TArray<AActor*>& InAvailableActors)
{
	float ClosestDistance = 0.f;
	// 가장 가까운 액터 찾아 반환
	return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, ClosestDistance);
}

void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	// Target Lock 위젯이 아직 생성되지 않았다면 실행
	if (!DrawnTargetLockWidget)
	{
		// 블루프린트에서 TargetLockWidgetClass 설정이 누락되었는지 확인
		// 설정되지 않았다면 런타임에서 에러 메시지 출력 후 종료
		checkf(TargetLockWidgetClass, TEXT("Forgot to assign a valid widget class in Blueprint"));

		// Hero의 컨트롤러를 Owning Player로 하여 Target Lock 위젯 생성
		DrawnTargetLockWidget = CreateWidget<UWarriorWidgetBase>(
			GetHeroControllerFromActorInfo(),	// 위젯의 Owning Player 지정
			TargetLockWidgetClass				// 위젯 클래스 (블루프린트에서 지정)
		);

		// 위젯 생성이 실제로 성공했는지 확인 (nullptr 방지)
		check(DrawnTargetLockWidget);

		// 생성된 위젯을 뷰포트에 추가하여 화면에 표시
		DrawnTargetLockWidget->AddToViewport();
	}
	
}

void UHeroGameplayAbility_TargetLock::SetTargetLockWidgetPosition()
{
	// 타겟 고정 위젯과 현재 고정된 액터 둘 중 하나라도 유효하지 않은 경우
	if (!DrawnTargetLockWidget || !CurrentLockedActor)
	{
		// 어빌리티 취소 및 리턴
		CancelTargetLockAbility();
		return;
	}

	// 화면 내에서 위젯이 표시될 2D 좌표
	FVector2D ScreenPosition;

	// 현재 락온된 액터의 월드 위치를 화면상의 좌표로 변환
	// (즉, 3D 공간에서의 액터 위치를 2D UI 화면 위치로 매핑)
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetHeroControllerFromActorInfo(),				// 투영 기준이 되는 플레이어 컨트롤러
		CurrentLockedActor->GetActorLocation(),			// 변환할 월드 위치 (현재 락온된 액터의 위치)
		ScreenPosition,                             // 결과로 받을 2D 스크린 좌표
		true											// 플레이어의 뷰포트 기준으로 위치 계산
	);

	// 위젯 크기가 아직 계산되지 않았다면 (처음 한 번만 계산하도록)
	if (TargetLockWidgetSize == FVector2D::ZeroVector)
	{
		// 위젯 트리 전체를 순회하며 내부 위젯을 검사
		DrawnTargetLockWidget->WidgetTree->ForEachWidget(
		   [this](UWidget* FoundWidget)
		   {
			  // SizeBox 위젯을 찾으면, 그 위젯의 설정된 Width/Height를 참조하여 크기 저장
			  if (USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
			  {
				 TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
				 TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
			  }
		   }
		);
	}

	// 계산된 크기를 기준으로, 위젯을 좌표의 중앙에 위치하도록 보정
	// ProjectWorldLocationToWidgetPosition은 좌상단 기준이므로
	// 위젯 크기의 절반만큼 좌표를 이동시켜 정중앙 정렬
	ScreenPosition -= (TargetLockWidgetSize / 2.f);
	
	// 계산된 화면 좌표로 타겟 락 위젯 위치를 갱신
	// false = UI 정렬(offset) 적용 안 함 (정확한 좌표 그대로 사용)
	DrawnTargetLockWidget->SetPositionInViewport(ScreenPosition, false);
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMovement()
{
	// 기본 최대 이동 속도 캐싱
	CachedDefaultMaxWalkSpeed = GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;
	// 이동 속도를 타겟 고정 이동 속도로 설정
	GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockMaxWalkSpeed;
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMappingContext()
{
	// 로컬 플레이어 변수 할당
	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	// 향상된 입력 (Enhanced Input) 서브시스템 할당
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);

	// 서브시스템에 타겟 고정 매핑 컨텍스트 추가 (우선도 3)
	Subsystem->AddMappingContext(TargetLockMappingContext, 3);
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::CleanUp()
{
	// 액터 배열 비우기
	AvailableActorsToLock.Empty();

	// 현재 고정된 액터 nullptr로
	CurrentLockedActor = nullptr;

	// 위젯이 화면에 표시중이라면 제거
	if (DrawnTargetLockWidget)
	{
		DrawnTargetLockWidget->RemoveFromParent();
	}

	// 위젯 nullptr로
	DrawnTargetLockWidget = nullptr;

	// 사이즈 초기화
	TargetLockWidgetSize = FVector2D::ZeroVector;

	// 이동 속도 캐싱 변수 초기화
	CachedDefaultMaxWalkSpeed = 0.f;
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMovement()
{
	// 이동 속도가 캐싱 된 경우
	if (CachedDefaultMaxWalkSpeed > 0.f)
	{
		// 최대 이동 속도를 기본값으로 재설정
		GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
	}
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMappingContext()
{
	// 어빌리티 활성화 상태에서 종료 시 크래시 발생하는 문제 해결 위함
	// 종료 시 EndAbility 호출 -> 재설정 기능 작동 -> 종료 시엔 HeroController가 nullptr 이므로 크래시 발생 
	if (!GetHeroControllerFromActorInfo())
	{
		// HeroController가 유효하지 않는 경우 함수 종료하도록 설정
		return;
	}
	
	// 로컬 플레이어 변수 할당
	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	// 향상된 입력 (Enhanced Input) 서브시스템 할당
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);

	// 서브시스템에서 타겟 고정 매핑 컨텍스트 제거
	Subsystem->RemoveMappingContext(TargetLockMappingContext);
}