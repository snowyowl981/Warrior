// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "PawnUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangedDelegate, float, NewPercent);

/**
 * 
 */
UCLASS()
class WARRIOR_API UPawnUIComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	// 현재 체력 변화 시 호출할 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentHealthChanged;
	
	
};
