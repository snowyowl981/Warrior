// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_ExecuteTaskOnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskTickDelegate, float, DeltaTime);

/**
 * 
 */
UCLASS()
class WARRIOR_API UAbilityTask_ExecuteTaskOnTick : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_ExecuteTaskOnTick();
	
	// 1. 이 함수를 블루프린트에서 호출 가능하게 만듭니다. (비동기 노드로 생성됨)
	// "BlueprintInternalUseOnly = TRUE"는 이 함수가 단순 호출 노드가 아니라,
	// 실행 핀이 여러 개 달린 '비동기 액션(Latent)' 노드 형태로 사용됨을 의미합니다.
	UFUNCTION(BlueprintCallable, Category = "Warrior|AbilityTasks", meta=(HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_ExecuteTaskOnTick* ExecuteTaskOnTick(UGameplayAbility* OwningAbility);

	// 2. 엔진의 Tick 사이클에 맞춰 매 프레임 호출되는 함수입니다.
	// 실제 C++ 로직에서 Super::TickTask(DeltaTime)을 호출하고, 델리게이트를 방송(Broadcast)합니다.
	//~ Begin UGameplayTask Interface
	virtual void TickTask(float DeltaTime) override;
	//~ End UGameplayTask Interface

	// 3. 블루프린트 노드의 '실행 핀'이 될 델리게이트입니다.
	// 이 델리게이트가 Broadcast 되면, 블루프린트에 연결된 뒤쪽 노드들이 실행됩니다.
	// 즉, TickTask 안에서 이 녀석을 호출해주면 매 프레임 뒤쪽 로직이 실행됩니다.
	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskTickDelegate OnAbilityTickDelegate;
};
