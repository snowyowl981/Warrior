// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "WarriorTypes/WarriorStructTypes.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "WarriorAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// 어빌리티 입력이 눌렸을 때 호출되는 함수 (예: 공격 버튼 누름)
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);

	// 어빌리티 입력이 떼어졌을 때 호출되는 함수 (예: 공격 버튼에서 손 뗌)
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	// Blueprint에서 호출 가능하며, AbilitySet에 정의된 어빌리티들을 ApplyLevel 레벨로 부여
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability", meta = (ApplyLevel = "1"))
	void GrantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantAbilitySpecHandles);

	// 캐릭터에게 부여된 기본 무기 관련 어빌리티들을 제거하는 함수
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
	void RemoveGrantedHeroWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);
	
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);
	
};
