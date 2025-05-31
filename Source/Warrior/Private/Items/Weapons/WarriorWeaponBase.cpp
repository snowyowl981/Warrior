// SnowyOwl All Rights Reserved


#include "Items/Weapons/WarriorWeaponBase.h"
#include "Components/BoxComponent.h"

// Sets default values
AWarriorWeaponBase::AWarriorWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponColisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponColisionBox"));
	WeaponColisionBox->SetupAttachment(GetRootComponent());
	WeaponColisionBox->SetBoxExtent(FVector(20.f));
	WeaponColisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

