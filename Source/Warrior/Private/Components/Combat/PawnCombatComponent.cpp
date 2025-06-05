// SnowyOwl All Rights Reserved


#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "Components/BoxComponent.h"

#include "WarriorDebugHelper.h"

// 새로운 무기를 등록하고, 필요하면 현재 장착 무기로 설정하는 함수
void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AWarriorWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	// 이미 동일한 태그로 등록된 무기가 있다면 에러를 발생시킴 (중복 등록 방지)
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("A named %s has already been added as carried weapon"), *InWeaponTagToRegister.ToString());

	// 등록하려는 무기 객체가 유효한지 확인
	check(InWeaponToRegister);

	// 맵에 무기를 태그와 함께 등록
	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	// 등록할 무기를 (WeaponBase) 사용해 CombatComponent에서 적과 상호작용 시 사용할 콜백 함수 바인딩
	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

	// 장착된 무기로 등록해야 한다면 현재 장착 무기 태그를 설정
	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}

	// 디버그 메시지 출력
	// const FString WeaponString = FString::Printf(TEXT("A weapon named : %s has been registered using the tag %s"), *InWeaponToRegister->GetName(), *InWeaponTagToRegister.ToString());
	// Debug::Print(WeaponString);
}

// 태그를 기반으로 캐릭터가 보유 중인 무기를 반환하는 함수
AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	// 해당 태그가 맵에 존재하는 경우
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
	{
		// 무기 포인터를 찾아서 반환
		if (AWarriorWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}

	// 해당 태그의 무기를 찾지 못한 경우 nullptr 반환
	return nullptr;
}

// 현재 캐릭터가 장착 중인 무기를 반환하는 함수
AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	// 장착 중인 무기 태그가 유효하지 않으면 nullptr 반환
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	// 태그를 통해 실제 무기 객체를 찾아서 반환
	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	// 토글시킬 피해 타입이 현재 장비중인 무기인 경우
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		// 현재 장비중인 무기 지역변수 할당
		AWarriorWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();

		// 현재 장비중인 무기 유효성 체크
		check(WeaponToToggle);

		// 충돌이 필요한 경우 Collision 활성화
		if (bShouldEnable)
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		// 충돌이 필요치 않은 경우 Collision 비활성화
		else
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	// TODO : Handle body collision boxes
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}
