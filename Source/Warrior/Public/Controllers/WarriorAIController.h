// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WarriorAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorAIController : public AAIController
{
	GENERATED_BODY()

public:
	// 생성자
	AWarriorAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer());

	//~ Begin IGenericTeamAgentInterface Interface.
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~ End IGenericTeamAgentInterface Interface.
	
protected:
	// 시각, 청각 감지 기능뿐만 아니라 손상 등 다양한 감지 기능 지원 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* EnemyPerceptionComponent;

	// 시야 감지 파라미터 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	// 타겟 인식 상태가 바뀔 때 호출될 델리게이트
	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated( AActor* Actor, FAIStimulus Stimulus);
};
