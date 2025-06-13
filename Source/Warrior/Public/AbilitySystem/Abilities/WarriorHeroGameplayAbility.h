// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorHeroGameplayAbility.generated.h"

class AWarriorHeroCharacter;
class AWarriorHeroController;
class UHeroCombatComponent;

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorHeroGameplayAbility : public UWarriorGameplayAbility
{
	GENERATED_BODY()

public:
	// 현재 어빌리티(Ability)의 ActorInfo에서 영웅 캐릭터(AWarriorHeroCharacter) 할당
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AWarriorHeroCharacter*	GetHeroCharacterFromActorInfo();
	
	// ActorInfo에서 영웅 컨트롤러(AWarriorHeroController) 할당
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AWarriorHeroController*	GetHeroControllerFromActorInfo();

	// ActorInfo에서 영웅 컨트롤러(UHeroCombatComponent) 할당
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UHeroCombatComponent*	GetHeroCombatComponentFromActorInfo();

	// 영웅 대미지 효과 SpecHandle 생성 함수
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);
	

private:
	// TWeakObjectPtr : 언리얼 스마트 포인터 유형 (객체의 참조만 유지하고 싶을 때)
	// 캐싱된 캐릭터 포인터
	TWeakObjectPtr<AWarriorHeroCharacter> CachedWarriorHeroCharacter;

	// 캐싱된 컨트롤러 포인터
	TWeakObjectPtr<AWarriorHeroController> CachedWarriorHeroController;

};
