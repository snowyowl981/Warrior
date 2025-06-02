// SnowyOwl All Rights Reserved


#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorEnemyGameplayAbility.h"

void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	// EnemyCombatAbilities 배열이 비어 있지 않은지 확인
	if (!EnemyCombatAbilities.IsEmpty())
	{
		// EnemyCombatAbilities 배열에 있는 각 능력 클래스에 대해 반복
		for (const TSubclassOf<UWarriorEnemyGameplayAbility>& AbilityClass : EnemyCombatAbilities)
		{
			// 유효하지 않은 클래스인 경우 다음 반복으로 건너뜀
			if (!AbilityClass) continue;

			// 해당 능력 클래스를 기반으로 Gameplay Ability Spec을 생성
			FGameplayAbilitySpec AbilitySpec(AbilityClass);

			// 능력의 SourceObject를 능력을 부여할 대상의 아바타 액터로 설정
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();

			// 능력 레벨 설정
			AbilitySpec.Level = ApplyLevel;

			// Ability System Component에 능력 부여
			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
