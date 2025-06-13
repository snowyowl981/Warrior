// SnowyOwl All Rights Reserved


#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

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

FActiveGameplayEffectHandle UWarriorGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	// 대상 액터로부터 Ability System Component를 할당
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	// TargetASC가 유효하고, 전달된 GameplayEffectSpec 핸들이 유효한지 확인. 유효하지 않으면 실행 중지(Assertion).
	check(TargetASC && InSpecHandle.IsValid());

	// 현재 Actor의 Ability System Component를 통해, 지정된 EffectSpec을 대상(TargetASC)에 적용하고 결과를 반환
	return GetWarriorAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*InSpecHandle.Data,		// 적용할 GameplayEffectSpec
		TargetASC				// 효과를 적용할 대상의 Ability System Component
	);
}

FActiveGameplayEffectHandle UWarriorGameplayAbility::BP_ApplyEffectSpecHandleToTargetActor(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EWarriorSuccessType& OutSuccessType)
{
	// 실제 GameplayEffectSpecHandle을 대상 액터에 적용하는 로직을 실행하고 핸들을 반환받아 할당.
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

	// 효과가 성공적으로 적용되었는지 확인하여 OutSuccessType에 결과를 저장.
	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EWarriorSuccessType::Successful : EWarriorSuccessType::Failed;

	// 적용 결과 핸들을 반환.
	return ActiveGameplayEffectHandle;
}
