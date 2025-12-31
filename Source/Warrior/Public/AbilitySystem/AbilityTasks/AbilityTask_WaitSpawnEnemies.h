// Copyright 2025. SnowyOwl All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitSpawnEnemies.generated.h"

class AWarriorEnemyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitSpawnEnemiesDelegate, const TArray<AWarriorEnemyCharacter*>&, SpawnedEnemies);
/**
 * 
 */
UCLASS()
class WARRIOR_API UAbilityTask_WaitSpawnEnemies : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	// 어빌리티 태스크 생성을 위한 정적 팩토리 함수
	UFUNCTION(BlueprintCallable, Category = "Warrior|AbilityTasks", meta = (DisplayName = "Wait Gameplay Event And Spawn Enemies", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE", NumToSpawn = "1", RandomSpawnRadius = "200"))
	static UAbilityTask_WaitSpawnEnemies* WaitSpawnEnemies(
	   UGameplayAbility* OwningAbility, 
	   FGameplayTag EventTag,
	   TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn,
	   int32 NumToSpawn,
	   const FVector& SpawnOrigin,
	   float RandomSpawnRadius,
	   const FRotator& SpawnRotation
	);
    
	// 스폰 완료 시 호출되는 델리게이트 (성공)
	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate OnSpawnFinished;
    
	// 스폰 실패 시 호출되는 델리게이트
	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate DidNotSpawn;
	
	//~ Begin UGameplayTask Interface
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	//~ End UGameplayTask Interface
    
private:
	// 태스크 실행 시 사용하기 위해 캐싱된 설정값들
	FGameplayTag CachedEventTag;
	TSoftClassPtr<AWarriorEnemyCharacter> CachedSoftEnemyClassToSpawn;
	int32 CachedNumToSpawn;
	FVector CachedSpawnOrigin;
	float CachedRandomSpawnRadius;
	FRotator CachedSpawnRotation;
	FDelegateHandle DelegateHandle;
	
	void OnGameplayEventReceived(const FGameplayEventData* InPayload);
	void OnEnemyClassLoaded();
	
};
