// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "EnemyCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UEnemyCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	// 타겟 상호작용 델리게이트 콜백 함수
	virtual void OnHitTargetActor(AActor* HitActor) override;
};
