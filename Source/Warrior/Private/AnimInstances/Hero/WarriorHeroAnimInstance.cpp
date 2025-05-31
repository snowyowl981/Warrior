// SnowyOwl All Rights Reserved


#include "AnimInstances/Hero/WarriorHeroAnimInstance.h"
#include "Characters/WarriorHeroCharacter.h"

void UWarriorHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 캐릭터 nullptr체크 및 Hero 타입 캐스팅
	if (OwningCharacter)
	{
		OwningHeroCharacter = Cast<AWarriorHeroCharacter>(OwningCharacter);
	}
}

void UWarriorHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	// 가속 중일 때
	if (bHasAcceleration)
	{
		// 기본 상태 지속 경과 시간 초기화
		IdleElapsedTime = 0.f;
		// 유휴 상태 진입 X
		bShouldEnterRelaxState = false;
	}
	// 가속 중이 아닐 때
	else
	{
		// 기본 상태 지속 경과 시간 증가
		IdleElapsedTime += DeltaSeconds;
		// 기본 상태 지속 시간이 임계값 이상 시 유휴 상태 조건 만족 및 진입
		bShouldEnterRelaxState = (IdleElapsedTime >= EnterRelaxStateThreshold);
	}
}
