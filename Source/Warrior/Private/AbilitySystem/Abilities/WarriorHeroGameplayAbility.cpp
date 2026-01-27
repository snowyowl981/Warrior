// SnowyOwl All Rights Reserved


#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Controllers/WarriorHeroController.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "WarriorGameplayTags.h"

AWarriorHeroCharacter* UWarriorHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	// 캐싱된 히어로 캐릭터가 유효하지 않은 경우
	if (!CachedWarriorHeroCharacter.IsValid())
	{
		// 현재 액터에서 가져와 할당
		CachedWarriorHeroCharacter = Cast<AWarriorHeroCharacter>(CurrentActorInfo->AvatarActor);
	}

	// 유효한지 재검사 후 유효하면 반환, 유효하지 않으면 nullptr 반환
	return CachedWarriorHeroCharacter.IsValid()? CachedWarriorHeroCharacter.Get() : nullptr;
}

AWarriorHeroController* UWarriorHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	// 캐싱된 히어로 컨트롤러가 유효하지 않은 경우
	if (!CachedWarriorHeroController.IsValid())
	{
		// 현재 액터에서 가져와 할당
		CachedWarriorHeroController = Cast<AWarriorHeroController>(CurrentActorInfo->PlayerController);
	}

	// 유효한지 재검사 후 유효하면 반환, 유효하지 않으면 nullptr 반환
	return CachedWarriorHeroController.IsValid()? CachedWarriorHeroController.Get() : nullptr;
}

UHeroCombatComponent* UWarriorHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	// 현재 어빌리티의 ActorInfo에서 영웅 캐릭터를 가져온 후, 영웅 전투 컴포넌트 반환
	return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}

FGameplayEffectSpecHandle UWarriorHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag,
	int32 InUsedComboCount)
{
	// 게임플레이 이펙트 유효성 체크
	check(EffectClass);

	// 컨텍스트 핸들 지역변수 할당 및 프로퍼티 설정
	FGameplayEffectContextHandle ContextHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
	
	// 이펙트 스펙 핸들 지역변수 할당 및 프로퍼티 설정
	FGameplayEffectSpecHandle EffectSpecHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	// 이벤트 스펙 핸들 데이터에 대미지 설정
	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		WarriorGameplayTags::Shared_SetByCaller_BaseDamage,
		InWeaponBaseDamage
	);

	// InCurrentAttackTypeTag가 유효한 경우 이벤트 스펙 핸들 데이터에 공격 유형과 콤보 카운트 설정
	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InUsedComboCount);
	}
	
	// 이펙트 스펙 핸들 반환
	return EffectSpecHandle;
}

bool UWarriorHeroGameplayAbility::GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime)
{
	// InCooldownTag가 유효한지(비어 있지 않은지) 확인.
	// 잘못된 태그로 쿨다운을 조회하는 버그를 방지.
	check(InCooldownTag.IsValid());

	// InCooldownTag를 가진 어떤 GameplayEffect든 매칭되는 쿨다운 쿼리 생성.
	// MakeQuery_MatchAnyOwningTags: 지정한 태그를 Owning Tag로 가진 효과를 찾는 쿼리.
	FGameplayEffectQuery CooldownQuery =
		FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());

	// 쿨다운 쿼리에 해당하는 활성 GameplayEffect들의
	// (남은 시간, 전체 지속 시간)을 전부 가져옴.
	// 반환 형식: TArray<TPair<RemainingTime, TotalDuration>>
	TArray<TPair<float, float>> TimeRemainingAndDuration =
		GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

	// 하나라도 해당 쿨다운 효과가 활성화되어 있으면 처리
	if (!TimeRemainingAndDuration.IsEmpty())
	{
		// 첫 번째 효과의 남은 시간 값을 읽어서 RemainingCooldownTime에 저장.
		// (여기서는 가장 대표/우선 효과 하나만 사용하는 셈)
		RemainingCooldownTime = TimeRemainingAndDuration[0].Key;

		// 전체 쿨다운 시간을 덮어쓰고 싶어서 의도한 것이라면,
		// TimeRemainingAndDuration[0].Value에 TotalCooldownTime을 넣고 있음.
		// 다만 이 배열은 복사본이라, 실제 서버에 반영되지는 않음.
		TimeRemainingAndDuration[0].Value = TotalCooldownTime;
	}

	// 남은 쿨다운 시간이 0보다 크면 아직 쿨다운 중이므로 true 반환,
	// 쿨다운이 없거나 끝났으면 false 반환.
	return RemainingCooldownTime > 0.f;

}
