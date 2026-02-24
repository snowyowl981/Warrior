// Copyright 2025. SnowyOwl All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "HeroGameplayAbility_PickUpStones.generated.h"

class AWarriorStoneBase;
/**
 * 
 */
UCLASS()
class WARRIOR_API UHeroGameplayAbility_PickUpStones : public UWarriorHeroGameplayAbility
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface
	// 어빌리티 활성화 및 종료
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface
	
	UFUNCTION(BlueprintCallable)
	void CollectStones();
	
private:
	// 스톤 박스 트레이스 거리
	UPROPERTY(EditDefaultsOnly)
	float BoxTraceDistance = 50.0f;
	
	// 트레이스 박스 크기
	UPROPERTY(EditDefaultsOnly)
	FVector TraceBoxSize = FVector(100.0f);
	
	// 트레이스 채널
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<	EObjectTypeQuery> > StoneTraceChannel;
	
	// 디버그 그릴지 여부
	UPROPERTY(EditDefaultsOnly)
	bool bDrawDebugShape = false;
	
	// 획득한 스톤
	UPROPERTY()
	TArray<AWarriorStoneBase*> CollectedStones;
};
