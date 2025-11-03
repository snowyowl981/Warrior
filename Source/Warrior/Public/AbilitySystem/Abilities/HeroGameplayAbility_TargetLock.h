// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "HeroGameplayAbility_TargetLock.generated.h"

class UWarriorWidgetBase;

/**
 * 
 */
UCLASS()
class WARRIOR_API UHeroGameplayAbility_TargetLock : public UWarriorHeroGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface
	// 어빌리티 활성화 및 종료
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface
	
private:
	// 타겟 고정 시도
	void TryLockOnTarget();
	// 고정할 유효 액터들 배열 설정
	void GetAvailableActorsToLock();
	// 유효한 액터 중 가장 가까운 액터 반환
	AActor* GetNearestTargetFromAvailableActor(const TArray<AActor*>& InAvailableActors);
	// 타겟 고정 위젯 그리기
	void DrawTargetLockWidget();
	// 타겟 고정 위젯 위치 설정
	void SetTargetLockWidgetPosition();

	// 타겟 고정 어빌리티 취소
	void CancelTargetLockAbility();
	// 고정 어빌리티 취소 시 정리
	void CleanUp();

	//~ Begin 박스 트레이스 충돌 탐지용 변수
	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float BoxTraceDistance = 5000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TArray<TEnumAsByte<EObjectTypeQuery> > BoxTraceChannel;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	bool bShowPersistentDebugShape = false;
	//~ End 박스 트레이스 충돌 탐지용 변수

	// 블루프린트에서 지정할 타겟 고정 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TSubclassOf<UWarriorWidgetBase> TargetLockWidgetClass;

	// 현재 고정 가능한 액터들을 저장하는 배열
	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;

	// 현재 고정 중인 액터 (플레이어가 타겟으로 고정한 대상)
	UPROPERTY()
	AActor* CurrentLockedActor;

	// 실제로 화면에 표시되고 있는 타겟 고정 위젯 인스턴스
	UPROPERTY()
	UWarriorWidgetBase* DrawnTargetLockWidget;

	UPROPERTY()
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;
};