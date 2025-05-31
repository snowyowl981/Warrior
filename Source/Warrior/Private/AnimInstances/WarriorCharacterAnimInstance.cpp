// SnowyOwl All Rights Reserved


#include "AnimInstances/WarriorCharacterAnimInstance.h"
#include "Characters/WarriorBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// 애니메이션 초기화 호출
void UWarriorCharacterAnimInstance::NativeInitializeAnimation()
{
	// 폰 가져오기 및 AWarriorBaseCharacter 타입 캐스팅
	OwningCharacter = Cast<AWarriorBaseCharacter>(TryGetPawnOwner());

	// nullptr체크 (초기화 함수이므로 매크로대신 if체크 사용)
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
