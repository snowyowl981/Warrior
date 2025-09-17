// SnowyOwl All Rights Reserved

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"

#include "WarriorDebugHelper.h"

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

bool UWarriorAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	// 전달받은 태그가 유효한지 체크 (에디터/개발 환경에서만 보장, 실패 시 crash)
	check(AbilityTagToActivate.IsValid());

	// 해당 태그와 매칭되는 어빌리티 스펙을 검색
	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(
		AbilityTagToActivate.GetSingleTagContainer(),		// 검색할 태그 컨테이너
		FoundAbilitySpecs								// 결과로 채워질 배열
	);

	// IsEmpty가 아니면 진입
	if (!FoundAbilitySpecs.IsEmpty())
	{
		// 매칭된 어빌리티 중 무작위 선택
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

		// nullptr 방지 체크
		check(SpecToActivate);

		// 이미 활성화 중인 어빌리티가 아니라면 실행 시도
		if (!SpecToActivate->IsActive())
		{
			return TryActivateAbility(SpecToActivate->Handle);
		}
	}

	// 조건에 맞는 어빌리티를 찾지 못했거나 실행 실패 시 false 반환
	return false;
}
