// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "WarriorTypes/WarriorStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "WarriorHeroWeapon.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorHeroWeapon : public AWarriorWeaponBase
{
	GENERATED_BODY()
	
public:
	// Hero 무기 데이터 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FWarriorHeroWeaponData HeroWeaponData;

	// 블루프린트에서 호출해서 GrantedAbilitySpecHandles 설정
	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	// 블루프린트에서 읽기 전용으로 GrantedAbilitySpecHandles 가져오기
	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;
	
private:
	// GrantedAbilitySpecHandles 멤버 TArray 변수
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
