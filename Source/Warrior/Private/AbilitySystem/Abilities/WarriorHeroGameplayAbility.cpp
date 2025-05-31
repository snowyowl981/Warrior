// SnowyOwl All Rights Reserved


#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Controllers/WarriorHeroController.h"
#include "Components/Combat/HeroCombatComponent.h"

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
