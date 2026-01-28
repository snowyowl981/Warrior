// Copyright 2025. SnowyOwl All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorPickUpBase.generated.h"

class USphereComponent;

UCLASS()
class WARRIOR_API AWarriorPickUpBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarriorPickUpBase();
	
protected:
	// 픽업 콜리전 구체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pick Up Interaction")
	USphereComponent* PickUpCollisionSphere;
	
	// 픽업 콜리전이 다른 컴포넌트와 겹침 시작 시 전달할 콜백 함수, UFUNCTION을 사용해야 델리게이트에 바인딩 가능
	UFUNCTION()
	virtual void OnPickupCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); 

};
