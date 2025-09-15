// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/WarriorBaseAnimInstance.h"
#include "WarriorCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorCharacterAnimInstance : public UWarriorBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	// AnimInstance 초기화 시 호출
	virtual void NativeInitializeAnimation() override;

	// 애니메이션 스레드에서 주기적으로 호출됨
	// NativeUpdateAnimation에서 처리한 데이터를 기반으로 애니메이션을 갱신
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	// 소유 중인 캐릭터 참조
	UPROPERTY()
	class AWarriorBaseCharacter* OwningCharacter;

	// 캐릭터의 MovementComponent 참조
	UPROPERTY()
	class UCharacterMovementComponent* OwningMovementComponent;

	// 지상 이동 속도
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	// 가속 여부
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;

	// Locomotion 이동 방향
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;


};
