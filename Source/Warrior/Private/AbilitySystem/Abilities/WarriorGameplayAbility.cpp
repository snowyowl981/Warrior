// SnowyOwl All Rights Reserved


#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"

// 어빌리티가 캐릭터(Actor)에게 부여될 때 호출되는 함수
void UWarriorGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	// 어빌리티 활성화 정책이 OnGiven일 경우, 즉시 어빌리티를 자동으로 활성화함
	if (AbilityActivationPolicy == EWarriorAbilityActivationPolicy::OnGiven)
	{
		// 유효한 ActorInfo가 있고, 어빌리티가 아직 활성 상태가 아닐 경우
		if (ActorInfo && !Spec.IsActive())
		{
			// 해당 어빌리티를 즉시 활성화 시도
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

// 어빌리티가 종료될 때 호출되는 함수
void UWarriorGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// 어빌리티 활성화 정책이 OnGiven인 경우, 어빌리티 종료 시 해당 스펙을 AbilitySystem에서 제거
	if (AbilityActivationPolicy == EWarriorAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

// ActorInfo로부터 PawnCombatComponent를 가져오는 유틸리티 함수
UPawnCombatComponent* UWarriorGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

// ActorInfo로부터 Warrior 전용 AbilitySystemComponent를 가져오는 유틸리티 함수
class UWarriorAbilitySystemComponent* UWarriorGameplayAbility::GetWarriorAbilitySystemComponentFromActorInfo() const
{
	return Cast<UWarriorAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}
