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
	// ======================================================
	// 쿨다운(Cooldown) 상태 확인 함수
	// ------------------------------------------------------
	// 특정 쿨다운 태그(InCooldownTag)가 적용된 GameplayEffect의
	// 남은 시간과 총 시간을 확인하여 쿨다운 상태 반환
	// ======================================================
	check(InCooldownTag.IsValid());  // 쿨다운 태그가 유효한지 안전 확인

	// --------------------------------------------------
	// [1] 쿨다운 태그를 가진 모든 GameplayEffect를 찾는 쿨리(Query) 생성
	// MakeQuery_MatchAnyOwningTags : 이 태그를 "소유"한 효과들을 찾음
	// (OwningTags = GameplayEffect가 적용된 액터가 가진 태그)
	// --------------------------------------------------
	FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
		InCooldownTag.GetSingleTagContainer());

	// --------------------------------------------------
	// [2] 쿨다운 쿨리와 매칭되는 **활성(active) GameplayEffect**들의
	// 남은 시간(TimeRemaining)과 총 지속시간(Duration)을 모두 가져옴
	// 반환: TArray<TPair<float, float>> (Key=남은시간, Value=총시간)
	// --------------------------------------------------
	TArray<TPair<float, float>> TimeRemainingAndDuration = 
		GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

	if (!TimeRemainingAndDuration.IsEmpty())
	{
		// --------------------------------------------------
		// [3] 첫 번째(보통 가장 최근 적용된) 쿨다운 효과의 시간 정보 추출
		// 실제 게임에서는 여러 쿨다운이 겹칠 수 있으므로 [0]번째 사용
		// --------------------------------------------------
		RemainingCooldownTime = TimeRemainingAndDuration[0].Key;   // 남은 쿨다운 시간
		TotalCooldownTime    = TimeRemainingAndDuration[0].Value;  // 쿨다운 총 지속시간
	}

	return RemainingCooldownTime > 0.f;  // 쿨다운 남은 시간이 0초 초과 → true(쿨다운 중)


}
