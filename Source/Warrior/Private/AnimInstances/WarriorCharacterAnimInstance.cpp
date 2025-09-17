// SnowyOwl All Rights Reserved


#include "AnimInstances/WarriorCharacterAnimInstance.h"
#include "Characters/WarriorBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

// 애니메이션 인스턴스 초기화 시 호출
void UWarriorCharacterAnimInstance::NativeInitializeAnimation()
{
	// 현재 이 AnimInstance가 참조하는 Pawn을 가져와서 WarriorBaseCharacter로 캐스팅
	OwningCharacter = Cast<AWarriorBaseCharacter>(TryGetPawnOwner());

	// nullptr 체크 (초기화 단계이므로 존재하지 않을 수도 있어 방어 코드 필요)
	if (OwningCharacter)
	{
		// 캐릭터의 MovementComponent 참조 캐싱
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

// 멀티스레드 환경에서 매 프레임마다 호출되어 애니메이션 데이터를 갱신
void UWarriorCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	// 캐릭터나 MovementComponent가 유효하지 않으면 리턴
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}

	// 2D 평면상의 속도 크기 (이동 속도)
	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	// 현재 가속 중인지 여부 (가속 벡터의 크기가 0보다 크면 true)
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;

	// 스트레이핑 방향 계산
	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());
}
