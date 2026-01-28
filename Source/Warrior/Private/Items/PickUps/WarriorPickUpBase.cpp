// Copyright 2025. SnowyOwl All Rights Reserved.


#include "Items/PickUps/WarriorPickUpBase.h"
#include "Components/SphereComponent.h"


AWarriorPickUpBase::AWarriorPickUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// 픽업 구체 생성 및 초기 설정 (반지름, 델리게이트)
	PickUpCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpCollisionSphere"));
	SetRootComponent(PickUpCollisionSphere);
	PickUpCollisionSphere->InitSphereRadius(50.0f);
	PickUpCollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnPickupCollisionSphereBeginOverlap);

}

void AWarriorPickUpBase::OnPickupCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
}

