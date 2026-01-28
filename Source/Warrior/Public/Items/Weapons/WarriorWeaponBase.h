// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorWeaponBase.generated.h"

// 전방 선언
class UBoxComponent;

// 싱글 캐스트 델리게이트 선언 매크로 (타겟과의 상호작용 델리게이트)
DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*)

UCLASS()
class WARRIOR_API AWarriorWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarriorWeaponBase();

	// Begin Overlap을 위한 델리게이트
	FOnTargetInteractedDelegate OnWeaponHitTarget;
	// End Overlap을 위한 델리게이트
	FOnTargetInteractedDelegate OnWeaponPulledFromTarget;
protected:
	// 무기 메쉬 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UStaticMeshComponent* WeaponMesh;

	// 무기 콜리전 박스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UBoxComponent* WeaponCollisionBox;

	// 무기가 다른 컴포넌트와 겹침 시작 시 전달할 콜백 함수, UFUNCTION을 사용해야 델리게이트에 바인딩 가능
	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 무기가 다른 컴포넌트와 겹침 끝날 시 전달할 콜백 함수, UFUNCTION을 사용해야 델리게이트에 바인딩 가능
	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

public:
	// 무기 콜리전 박스 가져오는 Getter
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox; }
};
