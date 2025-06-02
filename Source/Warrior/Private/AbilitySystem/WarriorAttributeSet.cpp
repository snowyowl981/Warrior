// SnowyOwl All Rights Reserved


#include "AbilitySystem/WarriorAttributeSet.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	// 생성 시 속성값 초기화
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}
