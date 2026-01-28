// SnowyOwl All Rights Reserved


#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"

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

void UWarriorGameplayAbility::ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResults)
{
	// 맞은 결과가 없으면 함수 종료
	if (InHitResults.IsEmpty())
	{
		return;
	}
	
	// 어빌리티 수행 액터로부터 폰 캐스팅, 체크 후 할당
	APawn* OwningPawn = CastChecked<APawn>(GetAvatarActorFromActorInfo());
	
	// InHitResults 배열의 각 히트 결과를 순회
	for (const FHitResult& Hit : InHitResults)
	{
		// 충돌한 액터가 폰(APawn)인지 확인 후 캐스팅
		if (APawn* HitPawn = Cast<APawn>(Hit.GetActor()))
		{
			// 대상 폰이 아군이 아닌, 적대적인 대상인지 확인
			if (UWarriorFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn))
			{
				// 지정된 GameplayEffectSpecHandle을 적 폰(HitPawn)에 적용
				// 이 함수는 효과를 실제로 타겟에게 부여하고, 그 결과를 핸들로 반환
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(HitPawn, InSpecHandle);
          
				// Gameplay 이벤트 데이터 구조체를 생성하고, 가해자/대상 정보를 설정
				FGameplayEventData Data;
				Data.Instigator = OwningPawn;	// 이펙트를 적용한 주체
				Data.Target = HitPawn;			// 이펙트가 적용된 대상
          
				// 효과가 성공적으로 적용되었을 경우에만 실행
				if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
				{
					// 타겟 액터(HitPawn)에게 '피격 리액션' 관련 Gameplay 이벤트를 전송
					// 이 이벤트는 애니메이션 처리나 사운드, 추가 반응(넉백 등)을 트리거 가능
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						HitPawn,
						WarriorGameplayTags::Shared_Event_HitReact,	// "피격 리액션" 태그
						Data											// 이벤트 데이터 (가해자/대상 정보)
					);
				}
			}
		}
	}
}
