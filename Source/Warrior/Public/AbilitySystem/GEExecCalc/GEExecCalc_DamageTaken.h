// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEExecCalc_DamageTaken.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UGEExecCalc_DamageTaken : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	// Damage 계산용 Execution Calculation 클래스 생성자
	UGEExecCalc_DamageTaken();

	// 최종 대미지 계산을 위한 모든 관련 값 확보, GAS의 ExecutionCalculation에서 실제로 대미지를 계산하는 부분
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
