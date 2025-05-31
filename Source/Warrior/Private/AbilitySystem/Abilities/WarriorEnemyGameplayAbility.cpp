// SnowyOwl All Rights Reserved


#include "AbilitySystem/Abilities/WarriorEnemyGameplayAbility.h"
#include "Characters/WarriorEnemyCharacter.h"

AWarriorEnemyCharacter* UWarriorEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	// 캐싱된 적 캐릭터가 유효하지 않은 경우
	if (!CachedWarriorEnemyCharacter.IsValid())
	{
		// 현재 액터에서 가져와 할당
		CachedWarriorEnemyCharacter = Cast<AWarriorEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}

	// 다시 유효성 검사 후 유효하면 반환, 유효하지 않으면 nullptr 반환
	return CachedWarriorEnemyCharacter.IsValid() ? CachedWarriorEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UWarriorEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	// 현재 어빌리티의 ActorInfo에서 적 캐릭터(Enemy Character)를 가져온 후, 적 전투 컴포넌트 반환
	return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}
