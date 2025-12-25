// SnowyOwl All Rights Reserved


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorGameplayTags.h"
#include "WarriorFunctionLibrary.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Components/BoxComponent.h"

#include "WarriorDebugHelper.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	// 플레이어 액터가 태그 포함 시 리턴
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}
	
	// 미포함 시 태그 포함
	OverlappedActors.AddUnique(HitActor);
	
	// TODO::Implement block check
	
	// 가드 유효성 여부
	bool bIsValidBlock = false;
	
	// 플레이어 가드 중인지, 공격이 가드 불가인지 확인해 할당
	const bool bIsPlayerBlocking = UWarriorFunctionLibrary::NativeDoesActorHaveTag(HitActor, WarriorGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetOwningPawn(), WarriorGameplayTags::Enemy_Status_Unblockable);
	
	// 플레이어 가드 중, 가불기 아닐 때
	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		bIsValidBlock = UWarriorFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
	}
	
	// 이벤트 데이터 로컬 변수 선언 및 속성값 설정
	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;
	
	// 가드 유효 시
	if (bIsValidBlock)
	{
		// 액터로 게임플레이 이벤트 전송
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitActor,
			WarriorGameplayTags::Player_Event_SuccessfulBlock,
			EventData
		);
	}
	// 가드 무효 시 대미지 계산
	else
	{
		// 액터로 게임플레이 이벤트 전송
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			WarriorGameplayTags::Shared_Event_MeleeHit,
			EventData
		);
	}
}

void UEnemyCombatComponent::ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	// 폰으로부터 적 캐릭터 캐스팅 후 반환
	AWarriorEnemyCharacter* OwningEnemyCharacter = GetOwningPawn<AWarriorEnemyCharacter>();
	
	// 더블 체크
	check(OwningEnemyCharacter);
	
	// 양손 콜리전 박스 할당
	UBoxComponent* LeftHandCollisionBox = OwningEnemyCharacter->GetLeftHandCollisionBox();
	UBoxComponent* RightHandCollisionBox = OwningEnemyCharacter->GetRightHandCollisionBox();
	
	// 더블 체크
	check(LeftHandCollisionBox && RightHandCollisionBox);
	
	// 어느 쪽 손인지와 bShouldEnable 값에 따라 충돌 토글
	switch (ToggleDamageType)
	{
	case EToggleDamageType::LeftHand:
		LeftHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
	case EToggleDamageType::RightHand:
		RightHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
	default:
		break;
	}
	
	// 충돌 처리 이후 오버랩된 액터 배열 비우기
	if (!bShouldEnable)
	{
		OverlappedActors.Empty();
	}
}
