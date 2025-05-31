// SnowyOwl All Rights Reserved

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	// 태그 무효할 결우 리턴
	if (!InInputTag.IsValid())
	{
		return;
	}

	// 활성화할 수 있는 어빌리티 반복해서 살펴보고 가져오는 기능
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// 컨테이너에 태그가 존재하지 않으면 다음 루프로
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)) continue;

		TryActivateAbility(AbilitySpec.Handle);
		
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
}

void UWarriorAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantAbilitySpecHandles)
{
	// 무기 어빌리티 배열 비어있을 시 리턴
	if (InDefaultWeaponAbilities.IsEmpty())
	{
		return;
	}

	// InDefaultWeaponAbilities 배열에 있는 각 FWarriorHeroAbilitySet 구조체를 순회
	for (const FWarriorHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		// 현재 AbilitySet이 유효하지 않으면 루프 건너뜀
		if (!AbilitySet.IsValid()) continue;

		// AbilityToGrant (부여할 어빌리티 클래스)로 FGameplayAbilitySpec 생성
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);

		// 이 어빌리티의 소스 객체를 현재 AbilitySystemComponent의 아바타(주체 액터)로 설정
		AbilitySpec.SourceObject = GetAvatarActor();

		// 어빌리티 레벨 설정 (대개 캐릭터 레벨이나 특정 수치 기반)
		AbilitySpec.Level = ApplyLevel;

		// 어빌리티 스펙에 입력 태그(InputTag) 추가 (예: 공격/방어 키와 연결하기 위한 태그)
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);

		// AbilitySystemComponent에 어빌리티를 부여
		// GiveAbility(AbilitySpec);

		// 어빌리티를 부여하고 반환된 핸들을 배열에 저장 (중복 없이)
		OutGrantAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UWarriorAbilitySystemComponent::RemoveGrantedHeroWeaponAbilities(TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	// 제거할 능력 핸들 목록이 비어있으면 즉시 함수 종료
	if (InSpecHandlesToRemove.IsEmpty())
	{
		return;
	}

	// 각 능력 핸들에 대해 반복
	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		// 핸들이 유효한 경우에만 처리
		if (SpecHandle.IsValid())
		{
			// AbilitySystemComponent의 ClearAbility 함수를 호출하여 해당 능력 제거
			ClearAbility(SpecHandle);
		}
	}

	// 모든 능력 제거 후, 입력 배열을 비워서 사용 완료 표시
	InSpecHandlesToRemove.Empty();
}
