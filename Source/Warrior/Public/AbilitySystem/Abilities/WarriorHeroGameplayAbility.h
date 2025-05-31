// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorHeroGameplayAbility.generated.h"


/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorHeroGameplayAbility : public UWarriorGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	class AWarriorHeroCharacter* GetHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	class AWarriorHeroController* GetHeroControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	class UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

private:
	// TWeakObjectPtr : 언리얼 스마트 포인터 유형 (객체의 참조만 유지하고 싶을 때)
	// 캐싱된 캐릭터 포인터
	TWeakObjectPtr<AWarriorHeroCharacter> CachedWarriorHeroCharacter;

	// 캐싱된 컨트롤러 포인터
	TWeakObjectPtr<AWarriorHeroController> CachedWarriorHeroController;

};
