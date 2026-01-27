// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "GameplayTagContainer.h"
#include "HeroUIComponent.generated.h"


// 무기 아이콘 변경 시 브로드캐스트되는 델리게이트
// 매개변수: SoftWeaponIcon - 새로 장착된 무기의 아이콘(TSoftObjectPtr<UTexture2D>)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedDelegate, TSoftObjectPtr<UTexture2D>, SoftWeaponIcon);

// 어빌리티(능력) 슬롯 아이콘이 업데이트될 때 브로드캐스트되는 델리게이트
// 매개변수:
//   AbilityInputTag - 어빌리티를 구분하는 GameplayTag
//   SoftAbilityIconMaterial - 어빌리티 아이콘의 머티리얼(TSoftObjectPtr<UMaterialInterface>)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityIconSlotUpdatedDelegate, FGameplayTag, AbilityInputTag, TSoftObjectPtr<UMaterialInterface>, SoftAbilityIconMaterial);

// 어빌리티 쿨다운이 시작될 때 브로드캐스트되는 델리게이트
// 매개변수:
//   AbilityInputTag - 어빌리티 입력 태그
//   TotalCooldownTime - 전체 쿨다운 시간(초)
//   RemainingCooldownTime - 남은 쿨다운 시간(초)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAbilityCooldownBeginDelegate, FGameplayTag, AbilityInputTag, float, TotalCooldownTime, float, RemainingCooldownTime);


/**
 * 
 */
UCLASS()
class WARRIOR_API UHeroUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	// 현재 분노 변화 시 호출할 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentRageChanged;

	// 장비한 무기 변경 시 호출할 델리게이트
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnEquippedWeaponChangedDelegate OnEquippedWeaponChanged;
	
	// 어빌리티 아이콘 슬롯 업데이트 시 호출할 델리게이트
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAbilityIconSlotUpdatedDelegate OnAbilityIconSlotUpdated;
	
	// 어빌리티 사용 시 쿨타임 시작 호출할 델리게이트
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAbilityCooldownBeginDelegate OnAbilityCooldownBegin;
};
