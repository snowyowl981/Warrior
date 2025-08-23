// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "WarriorGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EWarriorAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};
/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	// 어빌리티의 발동 정책을 정의하는 변수
	// - OnInputPressed: 입력이 눌렸을 때 발동
	// - WhileInputActive: 입력이 유지되는 동안 반복 발동
	// - OnTriggered: 코드나 조건에 의해 수동으로 발동
	// 기본값은 OnTriggered, 주로 코드에서 직접 어빌리티를 실행할 때 사용
	UPROPERTY(EditDefaultsOnly, Category = "WarriorAbility")
	EWarriorAbilityActivationPolicy AbilityActivationPolicy = EWarriorAbilityActivationPolicy::OnTriggered;
	
	// 액터로부터 전투 컴포넌트 받아오는 함수
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	class UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	// 액터로부터 어빌리티 시스템 컴포넌트 받아오는 함수
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	class UWarriorAbilitySystemComponent* GetWarriorAbilitySystemComponentFromActorInfo() const;

	// 특정 대상에게 GameplayEffect를 적용하는 기능을 수행 및 ActiveGameplayEffectHandle 반환
	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	// 블루프린트에서 호출 가능한 함수로, 대상 액터에 GameplayEffectSpecHandle을 적용하고 성공 여부를 반환
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target", ExpandBoolAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTargetActor(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EWarriorSuccessType& OutSuccessType);
};
