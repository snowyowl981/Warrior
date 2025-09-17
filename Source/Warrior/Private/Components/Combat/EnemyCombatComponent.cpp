// SnowyOwl All Rights Reserved


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorGameplayTags.h"

#include "WarriorDebugHelper.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	// 플레이어 액터가 태그 포함 시 리턴
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}
	
	// 미포함 시 태그 포함
	OverlappedActors.AddUnique(HitActor);
	
	// TODO::Implement block check
	
	// 가드 유효성 여부
	bool bIsValidBlock = false;
	
	const bool bIsPlayerBlocking = false;			// 플레이어 가드 중인지 여부
	const bool bIsMyAttackUnblockable = false;		// 가드 불가 공격 여부
	
	// 플레이어 가드 중, 가불기 아닐 때
	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		// TODO::Check if the block is valid
	}
	
	// 이벤트 데이터 로컬 변수 선언 및 속성값 설정
	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;
	
	// 가드 유효 시
	if (bIsValidBlock)
	{
		// TODO::Handle successful block
	}
	// 가드 무효 시 대미지 계산
	else
	{
		// 액터로 게임플레이 이벤트 전송
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			WarriorGameplayTags::Shared_Event_MeleeHit,
			EventData
		);
	}
}
