// Copyright 2025. SnowyOwl All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/WarriorPickUpBase.h"
#include "WarriorStoneBase.generated.h"

class UWarriorAbilitySystemComponent;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorStoneBase : public AWarriorPickUpBase
{
	GENERATED_BODY()
	
public:
	// 엄마 쟤 돌 먹어
	void Consume(UWarriorAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel);
	
protected:
	// 픽업 콜리전이 다른 컴포넌트와 겹침 시작 시 전달할 콜백 함수, UFUNCTION을 사용해야 델리게이트에 바인딩 가능
	virtual void OnPickupCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Stone Consumed"))
	void BP_OnStoneConsumed();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;
};
