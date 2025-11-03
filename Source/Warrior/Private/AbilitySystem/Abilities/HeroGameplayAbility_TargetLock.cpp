// SnowyOwl All Rights Reserved


#include "AbilitySystem/Abilities/HeroGameplayAbility_TargetLock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "WarriorDebugHelper.h"

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	TryLockOnTarget();
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CleanUp();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();

	if (AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	CurrentLockedActor = GetNearestTargetFromAvailableActor(AvailableActorsToLock);

	if (CurrentLockedActor)
	{
		Debug::Print(CurrentLockedActor->GetActorNameOrLabel());
	}
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

				// 디버그용으로 액터 이름 출력
				Debug::Print(HitActor->GetActorNameOrLabel());
			}
		}
	}
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailableActor(const TArray<AActor*>& InAvailableActors)
{
	float ClosestDistance = 0.f;
	return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, ClosestDistance);
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::CleanUp()
{
	AvailableActorsToLock.Empty();

	CurrentLockedActor = nullptr;
}