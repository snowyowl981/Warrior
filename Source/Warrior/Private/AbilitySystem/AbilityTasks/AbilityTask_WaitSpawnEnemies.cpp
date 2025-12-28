// Copyright 2025. SnowyOwl All Rights Reserved.


#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"

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
