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
	// AnimInstance 초기화 될 때 호출
	virtual void NativeInitializeAnimation() override;

	// 애니메이션 쓰레드에서 동작, NativeUpdateAnimation에서 설정한 정보를 처리하여 애니메이션을 설정
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	// 베이스 캐릭터 할당 변수
	class AWarriorBaseCharacter* OwningCharacter;

	UPROPERTY()
	// 캐릭터 움직임 컴포넌트
	class UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	// 이동속도
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	// 가속 여부
	bool bHasAcceleration;

};
