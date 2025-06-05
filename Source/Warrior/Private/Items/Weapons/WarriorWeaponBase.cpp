// SnowyOwl All Rights Reserved


#include "Items/Weapons/WarriorWeaponBase.h"
#include "Components/BoxComponent.h"

#include "WarriorDebugHelper.h"
// Sets default values
AWarriorWeaponBase::AWarriorWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 무기 메쉬 객체 생성 및 루트로 지정
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	
	// 무기 콜리전 박스 객채 생성, 루트에 부착, 기본 박스 사이즈 설정, 충돌 비활성화
	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 컴포넌트 간 충돌 시작 및 종료 시 호출 이벤트, 다이나믹에 델리게이트 함수 추가
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);
	
}

void AWarriorWeaponBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 원하는 유형(APawn)으로 형변환할 수 있는 템플릿 함수
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	// 무기를 소유한 Pawn 유효성 체크
	checkf(WeaponOwningPawn, TEXT("Forgot to assign an instigator as the owning pawn for the weapon : %s"), *GetName());

	// 무기를 소유한 Pawn이 다른 Pawn을 타격했을 경우
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		// 플레이어에만 적용, 무기를 소유한 Pawn이 맞은 Pawn이 아닌 경우
		if (WeaponOwningPawn != HitPawn)
		{
			// ExecuteIfBound : 싱글캐스트 델리게이트에 바인딩된 함수를 확인하고 존재하면 함수를 실행
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
		}

		// TODO : Implement hit check for enemy Characters
	}
}

void AWarriorWeaponBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 원하는 유형(APawn)으로 형변환할 수 있는 템플릿 함수
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	// 무기를 소유한 Pawn 유효성 체크
	checkf(WeaponOwningPawn, TEXT("Forgot to assign an instigator as the owning pawn for the weapon : %s"), *GetName());

	// 무기를 소유한 Pawn이 다른 Pawn을 타격했을 경우
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		// 플레이어에만 적용, 무기를 소유한 Pawn이 맞은 Pawn이 아닌 경우
		if (WeaponOwningPawn != HitPawn)
		{
			OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
		}

		// TODO : Implement hit check for enemy Characters
	}
}

