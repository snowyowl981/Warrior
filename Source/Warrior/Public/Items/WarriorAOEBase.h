// Copyright 2025. SnowyOwl All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "WarriorAOEBase.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;
struct FGameplayEventData;

UENUM(BlueprintType)
enum class EAOEDamagePolicy : uint8
{
	OnHit,			// 충돌
	OnBeginOverlap	// 겹침
};

UCLASS()
class WARRIOR_API AWarriorAOEBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarriorAOEBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// 광역 공격 충돌 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AOE")
	UCapsuleComponent* AOECollisionCapsule;
	
	// 광역 공격 시각적 표현 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AOE")
	UNiagaraComponent* AOENiagaraComponent;
	
	// 광역 공격 대미지 정책
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AOE")
	EAOEDamagePolicy AOEDamagePolicy = EAOEDamagePolicy::OnBeginOverlap;
	
	// 광역 공격 대미지 이펙트 스펙 핸들
	UPROPERTY(BlueprintReadOnly, Category = "AOE", meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle AOEDamageEffectSpecHandle;
	
	// 광역 공격 충돌 시 호출할 델리게이트
	UFUNCTION()
	virtual void OnAOEBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// 블루프린트 구현 가능 발사체 히트 FX
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Spawn AOE Hit FX"))
	void BP_OnSpawnAOEHitFX(const FVector& HitLocation);
	
private:
	// 발사체 대미지 처리 함수
	void HandleApplyAOEDamage(APawn* InHitPawn, const FGameplayEventData& InPayload);

};
