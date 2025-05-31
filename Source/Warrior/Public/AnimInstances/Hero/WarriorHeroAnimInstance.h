// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/WarriorCharacterAnimInstance.h"
#include "WarriorHeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorHeroAnimInstance : public UWarriorCharacterAnimInstance
{
	GENERATED_BODY()

public:
	// AnimInstance 초기화 될 때 호출
	virtual void NativeInitializeAnimation() override;

	// 애니메이션 쓰레드에서 동작, NativeUpdateAnimation에서 설정한 정보를 처리하여 애니메이션을 설정
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	// 영웅 캐릭터 할당 변수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|References")
	class AWarriorHeroCharacter* OwningHeroCharacter;

	// 유휴 상태 진입 여부
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;
	
	// 유휴 상태 진입 임계 시간값
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxStateThreshold = 5.f;

	// 기본 상태 경과 시간
	UPROPERTY()
	float IdleElapsedTime;
	
};
