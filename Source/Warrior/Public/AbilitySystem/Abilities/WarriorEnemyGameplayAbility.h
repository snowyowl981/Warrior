// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorEnemyGameplayAbility.generated.h"

// 전방 선언
class AWarriorEnemyCharacter;		// 적 캐릭터 클래스
class UEnemyCombatComponent;		// 적 전투 컴포넌트 클래스
/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorEnemyGameplayAbility : public UWarriorGameplayAbility
{
	GENERATED_BODY()

public:
	// 현재 어빌리티(Ability)의 ActorInfo에서 적 캐릭터(AWarriorEnemyCharacter) 할당
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AWarriorEnemyCharacter* GetEnemyCharacterFromActorInfo();

	// ActorInfo에서 적의 전투 컴포넌트(UEnemyCombatComponent) 할당
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

private:
	// 적 캐릭터에 대한 캐시된 참조
	// TWeakObjectPtr은 약한 참조를 의미하며, 소유권을 갖지 않고 가비지 컬렉션에 안전하게 대응 가능
	// 자주 접근할 필요가 있을 때 ActorInfo를 매번 찾지 않기 위해 캐싱
	TWeakObjectPtr<AWarriorEnemyCharacter> CachedWarriorEnemyCharacter;
};
