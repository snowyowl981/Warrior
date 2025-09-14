// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_OrientToTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UBTService_OrientToTargetActor : public UBTService
{
	GENERATED_BODY()

	// Native Service
	UBTService_OrientToTargetActor();

	// ~Begin UBTNode Interface 에셋 초기화 함수
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;
	// ~End UBTNode Interface

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	// 타겟 액터 키
	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector InTargetActorKey;

	// 회전 보간 속도
	UPROPERTY(EditAnywhere, Category = "Target")
	float RotationInterpSpeed;
	
};
