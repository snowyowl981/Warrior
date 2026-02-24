// Copyright 2025. SnowyOwl All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/WarriorPickUpBase.h"
#include "WarriorStoneBase.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorStoneBase : public AWarriorPickUpBase
{
	GENERATED_BODY()
	
protected:
	// 픽업 콜리전이 다른 컴포넌트와 겹침 시작 시 전달할 콜백 함수, UFUNCTION을 사용해야 델리게이트에 바인딩 가능
	virtual void OnPickupCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
