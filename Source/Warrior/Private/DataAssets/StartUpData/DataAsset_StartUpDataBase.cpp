// SnowyOwl All Rights Reserved


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	check(InASCToGive);

	// 시작 시, 특정 조건에서 어빌리티 부여 함수 실행
	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);
}


void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& InAbilitiesToGive, UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	// 배열이 비어 있는 경우 실행 취소
	if (InAbilitiesToGive.IsEmpty())
	{
		return;
	}

	// 부여 할 어빌리티 배열 루프
	for (const TSubclassOf<UWarriorGameplayAbility>& Ability : InAbilitiesToGive)
	{
		// 기능이 유효하지 않으면 계속 진행해 다음 루프로
		if (!Ability) continue;

		// 어빌리티 스펙 할당 및 필드값 설정
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;


		// 어빌리티 부여
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
