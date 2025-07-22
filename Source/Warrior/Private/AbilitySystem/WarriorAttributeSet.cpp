// SnowyOwl All Rights Reserved


#include "AbilitySystem/WarriorAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "WarriorDebugHelper.h"

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

void UWarriorAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	// 속성 값이 현재 체력 가져오기 속성과 같으면 게임플레이 효과를 사용해 현재 체력 속성 수정중이란 의미
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		// 현재 체력 설정 최대값 : 최대 체력, 최소값 : 0
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);
	}

	// 속성 값이 현재 분노 가져오기 속성과 같으면 게임플레이 효과를 사용해 현재 분노 속성 수정중이란 의미
	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		// 현재 분노 설정 최대값 : 최대 분노, 최소값 : 0
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());

		SetCurrentRage(NewCurrentRage);
	}

	// 속성 값이 현재 받는 피해량 속성과 같으면 게임플레이 효과를 사용해 현재 받는 피해량 속성을 수정해 피해를 입히고 있다는 의미
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		// 기존 체력
		const float OldHealth = GetCurrentHealth();
		// 받은 대미지
		const float DamageDone = GetDamageTaken();
		// 대미지를 입은 현재 체력
		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

		// 현재 체력 설정
		SetCurrentHealth(NewCurrentHealth);

		const FString DebugString = FString::Printf(
			TEXT("Old Health : %f, Damage Done : %f, NewCurrentHealth : %f"),
			OldHealth,
			DamageDone,
			NewCurrentHealth
		);

		Debug::Print(DebugString, FColor::Red);

		// TODO::UI에 알림 표시
		
		// TODO::캐릭터 사망 처리
		if (NewCurrentHealth == 0.f)
		{
			
		}
	}
}
