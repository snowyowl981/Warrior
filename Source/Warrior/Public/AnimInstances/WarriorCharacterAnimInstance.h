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
	// AnimInstance �ʱ�ȭ �� �� ȣ��
	virtual void NativeInitializeAnimation() override;

	// �ִϸ��̼� �����忡�� ����, NativeUpdateAnimation���� ������ ������ ó���Ͽ� �ִϸ��̼��� ����
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	// ���̽� ĳ���� �Ҵ� ����
	class AWarriorBaseCharacter* OwningCharacter;

	UPROPERTY()
	// ĳ���� ������ ������Ʈ
	class UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	// �̵��ӵ�
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	// ���� ����
	bool bHasAcceleration;

};
