// Copyright 2025. SnowyOwl All Rights Reserved.


#include "Items/WarriorAOEBase.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

// Sets default values
AWarriorAOEBase::AWarriorAOEBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// 캡슐 컴포넌트 오브젝트 생성 및 루트 지정
	AOECollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AOECollisionCapsule"));
	SetRootComponent(AOECollisionCapsule);
	
	// 충돌 설정
	AOECollisionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AOECollisionCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	AOECollisionCapsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	AOECollisionCapsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	// 겹쳤을 시 호출할 델리게이트
	AOECollisionCapsule->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnAOEBeginOverlap);
	
	// 나이아가라 컴포넌트 생성, Root에 부착
	AOENiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("ProjectileNiagaraComponent");
	AOENiagaraComponent->SetupAttachment(GetRootComponent());
	
	// 2초 후 제거
	InitialLifeSpan = 2.0f;
	
}

// Called when the game starts or when spawned
void AWarriorAOEBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 발사체가 대상 폰과 겹쳤을 때 설정
	if (AOEDamagePolicy == EAOEDamagePolicy::OnBeginOverlap)
	{
		AOECollisionCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

void AWarriorAOEBase::OnAOEBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 광역 공격 히트 시 이펙트 스폰, 충격음 재생
	BP_OnSpawnAOEHitFX(SweepResult.ImpactPoint);
	
	// 피격된 폰 할당
	APawn* HitPawn = Cast<APawn>(OtherActor);
	
	// 광역 공격이 명중한 대상이 적대적이지 않거나 명중 대상이 유효하지 않은 경우 리턴
	if (!HitPawn || !UWarriorFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn))
	{
		return;
	}
	
	// 데이터 설정
	FGameplayEventData Data;
	Data.Instigator = this;
	Data.Target = HitPawn;
	
	// 대미지 처리
	HandleApplyAOEDamage(HitPawn, Data);
}

void AWarriorAOEBase::HandleApplyAOEDamage(APawn* InHitPawn, const FGameplayEventData& InPayload)
{
	// 발사체 대미지 스펙 핸들이 유효한지 검증 (유효하지 않으면 에러 발생)
	checkf(AOEDamageEffectSpecHandle.IsValid(), TEXT("Forgot to assign a valid spec handle to the AOE : %s"), *GetActorNameOrLabel());
	
	// 시전자가 피격 대상에게 데미지 이펙트 적용 시도
	const bool bWasApplied = UWarriorFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), InHitPawn, AOEDamageEffectSpecHandle);
    
	// 이펙트 적용에 성공했을 경우
	if (bWasApplied)
	{
		// 피격 대상에게 피격 반응(Hit React) 게임플레이 이벤트 전달
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		   InHitPawn,
		   WarriorGameplayTags::Shared_Event_HitReact,
		   InPayload
		);
	}
}

