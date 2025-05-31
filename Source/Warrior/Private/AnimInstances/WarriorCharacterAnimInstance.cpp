// SnowyOwl All Rights Reserved


#include "AnimInstances/WarriorCharacterAnimInstance.h"
#include "Characters/WarriorBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// �ִϸ��̼� �ʱ�ȭ ȣ��
void UWarriorCharacterAnimInstance::NativeInitializeAnimation()
{
	// �� �������� �� AWarriorBaseCharacter Ÿ�� ĳ����
	OwningCharacter = Cast<AWarriorBaseCharacter>(TryGetPawnOwner());

	// nullptrüũ (�ʱ�ȭ �Լ��̹Ƿ� ��ũ�δ�� ifüũ ���)
	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UWarriorCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}

	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
}
