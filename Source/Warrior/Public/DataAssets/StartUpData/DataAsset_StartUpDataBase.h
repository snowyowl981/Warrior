// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UWarriorGameplayAbility;
class UWarriorAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class WARRIOR_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	// 어빌리티 시스템 컴포넌트 (ASC) 부여 함수
	virtual void GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
	// 시작 시 부여 할 어빌리티
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<	TSubclassOf	< UWarriorGameplayAbility > > ActivateOnGivenAbilities;
	
	// 특정 조건에서 부여 할 어빌리티
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<	TSubclassOf	< UWarriorGameplayAbility > > ReactiveAbilities;

	// 어빌리티 부여 함수
	void GrantAbilities(const TArray<	TSubclassOf	< UWarriorGameplayAbility > >& InAbilitiesToGive, UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
