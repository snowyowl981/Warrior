// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToFaceTarget.generated.h"

// 회전(Task) 실행 시 필요한 임시 메모리를 저장하는 구조체
struct FRotateToFaceTargetTaskMemory
{
	// 회전 주체가 되는 Pawn (소유자)
	TWeakObjectPtr<APawn> OwningPawn;

	// 바라볼 대상 액터
	TWeakObjectPtr<AActor> TargetActor;
	
	// Pawn과 TargetActor 둘 다 유효한지 검사
	bool IsValid() const
	{
		return OwningPawn.IsValid() && TargetActor.IsValid();
	}

	// 저장된 포인터들을 초기화(무효화)
	void Reset()
	{
		OwningPawn.Reset();
		TargetActor.Reset();
	}
};

/**
 * 
 */
UCLASS()
class WARRIOR_API UBTTask_RotateToFaceTarget : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_RotateToFaceTarget();

	//~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool HasReachedAnglePercision(APawn* QueryPawn, AActor* TargetActor) const;

	// 각 정밀도
	UPROPERTY(EditAnywhere, Category = "FaceTarget")
	float AnglePrecision;

	// 회전 보간 속도
	UPROPERTY(EditAnywhere, Category = "FaceTarget")
	float RotationInterpSpeed;

	// 마주할 타겟 키
	UPROPERTY(EditAnywhere, Category = "FaceTarget")
	FBlackboardKeySelector InTargetToFaceKey;
	
};
