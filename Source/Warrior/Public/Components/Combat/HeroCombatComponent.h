// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"

#include "HeroCombatComponent.generated.h"

class AWarriorHeroWeapon;
/**
 * 
 */
UCLASS()
class WARRIOR_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	// 태그로 영웅 무기 가져오는 함수
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorHeroWeapon* GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	// 현재 장비중인 영웅 무기 반환 함수
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorHeroWeapon* GetHeroCurrentEquippedWeapon() const;

	// 레벨에 따라 현재 장비중인 영웅 무기 대미지 반환 함수
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	float GetHeroCurrentEquippedWeaponDamageAtLevel(float InLevel) const;
	
	// 타겟 상호작용 델리게이트 콜백 함수 (PawnCombatComponent로부터 상속)
	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;
};
