// SnowyOwl All Rights Reserved


#include "Items/Weapons/WarriorHeroWeapon.h"

// GrantedAbilitySpecHandles 설정
void AWarriorHeroWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitySpecHandles = InSpecHandles;
}

// GrantedAbilitySpecHandles 가져오기
TArray<FGameplayAbilitySpecHandle> AWarriorHeroWeapon::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}
