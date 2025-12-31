// Copyright 2025. SnowyOwl All Rights Reserved.


#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"
#include "AbilitySystemComponent.h"
#include "WarriorDebugHelper.h"

UAbilityTask_WaitSpawnEnemies* UAbilityTask_WaitSpawnEnemies::WaitSpawnEnemies(UGameplayAbility* OwningAbility, FGameplayTag EventTag, TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn,
	int32 NumToSpawn, const FVector& SpawnOrigin, float RandomSpawnRadius, const FRotator& SpawnRotation)
{
	// 새 어빌리티 태스크 인스턴스 생성
	UAbilityTask_WaitSpawnEnemies* Node = NewAbilityTask<UAbilityTask_WaitSpawnEnemies>(OwningAbility);
    
	// 추후 실행을 위해 입력받은 파라미터 캐싱
	Node->CachedEventTag				= EventTag;
	Node->CachedSoftEnemyClassToSpawn	= SoftEnemyClassToSpawn;
	Node->CachedNumToSpawn				= NumToSpawn;
	Node->CachedSpawnOrigin             = SpawnOrigin;
	Node->CachedRandomSpawnRadius		= RandomSpawnRadius;
	Node->CachedSpawnRotation			= SpawnRotation;
    
	// 생성된 태스크 노드 반환
	return Node;
}

void UAbilityTask_WaitSpawnEnemies::Activate()
{
	// 캐시된 태그에 해당하는 게임플레이 이벤트 델리게이트 검색 또는 생성
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
    
	// 이벤트 수신 시 호출될 콜백 함수 등록 및 핸들 저장
	DelegateHandle = Delegate.AddUObject(this, &ThisClass::OnGameplayEventReceived);
}

void UAbilityTask_WaitSpawnEnemies::OnDestroy(bool bInOwnerFinished)
{
	// 등록했던 델리게이트에 다시 접근
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
    
	// 델리게이트 바인딩 해제
	Delegate.Remove(DelegateHandle);
    
	// 부모 클래스의 정리 작업 수행
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitSpawnEnemies::OnGameplayEventReceived(const FGameplayEventData* InPayload)
{
	Debug::Print(TEXT("Gameplay event received"));
	
	EndTask();
}
