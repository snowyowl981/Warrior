// Copyright 2025. SnowyOwl All Rights Reserved.


#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"
#include "AbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "NavigationSystem.h"
#include "Characters/WarriorEnemyCharacter.h"

#include "WarriorDebugHelper.h"

UAbilityTask_WaitSpawnEnemies* UAbilityTask_WaitSpawnEnemies::WaitSpawnEnemies(UGameplayAbility* OwningAbility, FGameplayTag EventTag, TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn,
                                                                               int32 NumToSpawn, const FVector& SpawnOrigin, float RandomSpawnRadius)
{
	// 새 어빌리티 태스크 인스턴스 생성
	UAbilityTask_WaitSpawnEnemies* Node = NewAbilityTask<UAbilityTask_WaitSpawnEnemies>(OwningAbility);
    
	// 추후 실행을 위해 입력받은 파라미터 캐싱
	Node->CachedEventTag				= EventTag;
	Node->CachedSoftEnemyClassToSpawn	= SoftEnemyClassToSpawn;
	Node->CachedNumToSpawn				= NumToSpawn;
	Node->CachedSpawnOrigin             = SpawnOrigin;
	Node->CachedRandomSpawnRadius		= RandomSpawnRadius;
    
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
    // 스폰할 적 클래스 정보가 유효한지 확인
    if (ensure(!CachedSoftEnemyClassToSpawn.IsNull()))
    {
       // 비동기 방식으로 적 클래스 로드 요청 및 완료 시 콜백 등록
       UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
       CachedSoftEnemyClassToSpawn.ToSoftObjectPath(),
       FStreamableDelegate::CreateUObject(this, &ThisClass::OnEnemyClassLoaded));
    }
    else
    {
       // 클래스 정보가 없으면 실패 델리게이트 방송 후 태스크 종료
       if (ShouldBroadcastAbilityTaskDelegates())
       {
          DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
       }
       
       EndTask();
    }
}

void UAbilityTask_WaitSpawnEnemies::OnEnemyClassLoaded()
{
    // 로드된 클래스 정보와 월드 가져오기
    UClass* LoadedClass = CachedSoftEnemyClassToSpawn.Get();
    UWorld* World = GetWorld();
    
    // 로드 실패 또는 월드가 없을 경우 예외 처리
    if (!LoadedClass || !World)
    {
       if (ShouldBroadcastAbilityTaskDelegates())
       {
          DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
       }
       
       EndTask();
       
       return;
    }
    
    TArray<AWarriorEnemyCharacter*> SPawnedEnemies;
    FActorSpawnParameters SpawnParam;
    SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
    // 설정된 마릿수만큼 반복 스폰
    for (int32 i = 0; i < CachedNumToSpawn; i++)
    {
		FVector RandomLocation;
		// 내비게이션 시스템을 이용해 주변의 도달 가능한 랜덤 위치 탐색
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, CachedSpawnOrigin, RandomLocation, CachedRandomSpawnRadius);
       
		// 바닥에 끼이지 않도록 스폰 높이 보정
		RandomLocation += FVector(0.0f, 0.0f, 150.0f);
    	
    	// 스폰 시 캐릭터 방향 회전
		const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();
       
		// 적 캐릭터 스폰 시도
		AWarriorEnemyCharacter* SpawnedEnemy = World->SpawnActor<AWarriorEnemyCharacter>(LoadedClass, RandomLocation, SpawnFacingRotation, SpawnParam);
    	
		if (SpawnedEnemy)
		{
			SPawnedEnemies.Add(SpawnedEnemy);
		}
    }
    
    // 스폰 결과에 따라 성공 또는 실패 델리게이트 브로드캐스트
    if (ShouldBroadcastAbilityTaskDelegates())
    {
       if (!SPawnedEnemies.IsEmpty())
       {
          OnSpawnFinished.Broadcast(SPawnedEnemies);
       }
       else
       {
          DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
       }
    }
    
    // 태스크 종료
    EndTask();
}
