// SnowyOwl All Rights Reserved


#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/WarriorHeroWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "WarriorGameplayTags.h"
#include "WarriorDebugHelper.h"

class AWarriorHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<AWarriorHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

AWarriorHeroWeapon* UHeroCombatComponent::GetHeroCurrentEquippedWeapon() const
{
	return Cast<AWarriorHeroWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UHeroCombatComponent::GetHeroCurrentEquippedWeaponDamageAtLevel(float InLevel) const
{
	return GetHeroCurrentEquippedWeapon()->HeroWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}

void UHeroCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	// 맞은 액터가 오버랩된 액터 배열에 포함된 경우 함수 종료 (한 번만 히트처리하기 위해)
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	// 오버랩된 액터에 맞은 액터 추가 (히트 처리)
	OverlappedActors.AddUnique(HitActor);

	// 전송할 게임플레이 이벤트 데이터 생성 및 값 설정
	FGameplayEventData	Data;
	Data.Instigator	= GetOwningPawn();
	Data.Target		= HitActor;
	
	// 액터에게 게임플레이 이벤트 전송 (액터, 물리 타격 태그, 이벤트 데이터 전달)
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		WarriorGameplayTags::Shared_Event_MeleeHit,
		Data
	);

	// 액터에게 HitPause 게임플레이 이벤트 전송
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		WarriorGameplayTags::Player_Event_HitPause,
		FGameplayEventData()
	);
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	// 타격이 끝날 때 발생시킬 다른 HitPause 효과
	// UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
	// 	GetOwningPawn(),
	// 	WarriorGameplayTags::Player_Event_HitPause,
	// 	FGameplayEventData()
	// );
}
