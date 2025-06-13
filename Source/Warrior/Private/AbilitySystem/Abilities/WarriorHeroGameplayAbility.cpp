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
	
	return EffectSpecHandle;
}
