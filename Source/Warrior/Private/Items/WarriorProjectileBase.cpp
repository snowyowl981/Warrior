// SnowyOwl All Rights Reserved


#include "Items/WarriorProjectileBase.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "WarriorDebugHelper.h"

// Sets default values
AWarriorProjectileBase::AWarriorProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 콜리전 박스 서브 오브젝트 생성 및 루트로 지정
	ProjectileCollisionBox = CreateDefaultSubobject<UBoxComponent>("ProjectileCollisionBox");
	SetRootComponent(ProjectileCollisionBox);
	
	// 충돌 설정
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	// 충돌 시, 겹쳤을 시 호출할 델리게이트
	ProjectileCollisionBox->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHit);
	ProjectileCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);
	
	// 나이아가라 컴포넌트 생성, Root에 부착
	ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("ProjectileNiagaraComponent");
	ProjectileNiagaraComponent->SetupAttachment(GetRootComponent());
	
	// 발사체 움직임 컴포넌트 생성 및 설정
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 700.0f;
	ProjectileMovementComponent->MaxSpeed = 900.0f;
	ProjectileMovementComponent->Velocity = FVector(1.0f, 0.0f, 0.0f);
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	
	// 4초 후 제거
	InitialLifeSpan = 4.0f;
	
}

// Called when the game starts or when spawned
void AWarriorProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 발사체가 대상 폰과 겹쳤을 때 설정
	if (ProjectileDamagePolicy == EProjectileDamagePolicy::OnBeginOverlap)
	{
		ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

void AWarriorProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 발사체 히트 시 이펙트 스폰, 충격음 재생
	BP_OnSpawnProjectileHitFX(Hit.ImpactPoint);
	
	// 맞은 폰 할당
	APawn* HitPawn = Cast<APawn>(OtherActor);
	
	// 발사체가 명중한 대상이 적대적이지 않거나 명중 대상이 유효하지 않은 경우
	if (!HitPawn || !UWarriorFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn))
	{
		// 투사체 파괴 후 함수 종료
		Destroy();
		return;
	}
	
	// 유효한 막기인지 확인
	bool bIsValidBlock = false;
	
	// 플레이어가 가드중인지 확인
	const bool bIsPlayerBlocking = UWarriorFunctionLibrary::NativeDoesActorHaveTag(HitPawn, WarriorGameplayTags::Player_Status_Blocking);
	
	if (bIsPlayerBlocking)
	{
		bIsValidBlock = UWarriorFunctionLibrary::IsValidBlock(this, HitPawn);
	}
	
	// 이벤트 데이터 설정
	FGameplayEventData Data;
	Data.Instigator = this;
	Data.Target = HitPawn;
	
	// 유효한 막기일 경우
	if (bIsValidBlock)
	{
		// 플레이어에게 가드 성공 이벤트 전달
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			WarriorGameplayTags::Player_Event_SuccessfulBlock,
			Data
		);
	}
	// 유효하지 않은 경우 피격 전달
	else
	{
		// 발사체 대미지 처리
		HandleApplyProjectileDamage(HitPawn, Data);
	}
	
	// 투사체 파괴
	Destroy();
}

void AWarriorProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedActors.Contains(OtherActor))
	{
		return;
	}
	
	OverlappedActors.AddUnique(OtherActor);
	
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		FGameplayEventData Data;
		Data.Instigator = GetInstigator();
		Data.Target = HitPawn;
		
		if (UWarriorFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn))
		{
			HandleApplyProjectileDamage(HitPawn, Data);
		}
	}
}

void AWarriorProjectileBase::HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload)
{
	// 투사체 데미지 스펙 핸들이 유효한지 검증 (유효하지 않으면 에러 발생)
	checkf(ProjectileDamageEffectSpecHandle.IsValid(), TEXT("Forgot to assign a valid spec handle to the projectile : %s"), *GetActorNameOrLabel());

	// 시전자가 피격 대상에게 데미지 이펙트 적용 시도
	const bool bWasApplied = UWarriorFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), InHitPawn, ProjectileDamageEffectSpecHandle);
    
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

