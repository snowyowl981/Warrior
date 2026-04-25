// Copyright 2025. SnowyOwl All Rights Reserved.


#include "GameModes/WarriorSurvivalGameMode.h"
#include "Engine/AssetManager.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "NavigationSystem.h"

#include "WarriorDebugHelper.h"

void AWarriorSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// 적 웨이브 스포너 데이터 테이블 확인
	checkf(EnemyWaveSpawnerDataTable, TEXT("Forgot to assign a valid data table"));
	
	// 게임모드 상태 설정 (새 웨이브 스폰 대기중)
	SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);
	
	// 데이터 테이블로부터 총 웨이브 수 할당
	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();
	
	// 다음 웨이브의 적 미리 로드
	PreLoadNextWaveEnemies();
}

void AWarriorSurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 게임모드 상태가 스폰 대기중인 경우
	if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::WaitSpawnNewWave)
	{
		// 경과 시간에 델타타임 더하기
		TimePassedSinceStart += DeltaTime;
		
		// 경과 시간이 스폰 대기 시간 이상이 된 경우
		if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
		{
			// 경과 시간 초기화
			TimePassedSinceStart = 0.0f;
			// 게임모드 상태 스폰중으로 설정
			SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::SpawningNewWave);
		}
	}
	
	// 게임모드 상태가 스폰 중인 경우
	if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::SpawningNewWave)
	{
		// 경과 시간에 델타타임 더하기
		TimePassedSinceStart += DeltaTime;
		
		// 경과 시간이 스폰 딜레이 이상인 경우
		if (TimePassedSinceStart >= SpawnEnemiesDelayTime)
		{
			CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
			TimePassedSinceStart = 0.0f;
			
			// 게임모드 상태 진행 중으로 설정
			SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::InProgress);
		}
	}
	
	// 게임모드 상태 웨이브 완료
	if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::WaveCompleted)
	{
		TimePassedSinceStart += DeltaTime;
		
		// 경과 시간이 웨이브 완료 후 대기 시간 이상인 경우
		if (TimePassedSinceStart >= WaveCompletedWaitTime)
		{
			// 경과 시간 초기화, 웨이브 카운트 증가
			TimePassedSinceStart = 0.0f;
			CurrentWaveCount++;
			
			// 모든 웨이브 끝낸 경우 게임모드 상태 설정
			if (HasFinishedAllWaves())
			{
				SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::AllWavesDone);
			}
			// 끝내지 못한 경우 게임모드 상태 설정 및 다음 웨이브 적 미리 로드
			else
			{
				SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);
				PreLoadNextWaveEnemies();
			}
		}
	}
}

void AWarriorSurvivalGameMode::SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState InState)
{
	// 현재 상태 설정
	CurrentSurvivalGameModeState = InState;
	// 델리게이트 브로드캐스트
	OnSurvivalGameModeStateChanged.Broadcast(CurrentSurvivalGameModeState);
}

bool AWarriorSurvivalGameMode::HasFinishedAllWaves() const
{
	// 현재 웨이브 카운트가 총 웨이브 수보다 많으면 클리어
	return CurrentWaveCount > TotalWavesToSpawn;
}

void AWarriorSurvivalGameMode::PreLoadNextWaveEnemies()
{
	// 모든 웨이브 종료 시 리턴
	if (HasFinishedAllWaves())
	{
		return;
	}
	
	// TMap 비우기
	PreLoadedEnemyClassMap.Empty();
	
	// 현재 웨이브의 스포너 정보(EnemyWaveSpawnerDefinitions)를 순회
	for (const FWarriorEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		// 소프트 레퍼런스가 비어 있으면(설정 안 됐으면) 건너뜀
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;
    
		// 애셋 매니저의 StreamableManager를 통해 해당 적 클래스 애셋을 비동기 로드 요청
		UAssetManager::GetStreamableManager().RequestAsyncLoad(
		   SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),   // 로드할 적 클래스의 경로(FSoftObjectPath)로 변환
		   FStreamableDelegate::CreateLambda(
			  [SpawnerInfo, this]()                                 // 로드 완료 후 호출될 람다 델리게이트
				 {
					// 비동기 로드가 끝난 뒤 실제 UClass* 를 가져옴
					if (UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
					{
					   // 미리 로드된 적 클래스 맵에 소프트 레퍼런스와 실제 클래스를 캐시
					   PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn, LoadedEnemyClass);
					}
				 }
			  )
		   );
	}
}

FWarriorEnemyWaveSpawnerTableRow* AWarriorSurvivalGameMode::GetCurrentWaveSpawnerTableRow() const
{
	// "Wave0", "Wave1" 같은 현재 웨이브용 Row 이름 생성
	const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));

	// 데이터 테이블에서 해당 Row 서치
	FWarriorEnemyWaveSpawnerTableRow* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FWarriorEnemyWaveSpawnerTableRow>(RowName, FString());

	// Row 를 못 찾으면 에러 발생
	checkf(FoundRow, TEXT("Could not find a valid row under the name %s in the data table"), *RowName.ToString());

	// 찾은 Row 반환
	return FoundRow;
}

int32 AWarriorSurvivalGameMode::TrySpawnWaveEnemies()
{
    // 현재 타겟 포인트 배열이 비어있다면 한 번만 월드에서 찾아와 캐싱
    if (TargetPointsArray.IsEmpty())
    {
        // 월드 내에 배치된 ATargetPoint 액터들을 전부 찾아 TargetPointsArray에 할당
        UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointsArray);
    }
    
    // 유효한 타겟 포인트가 하나도 없으면 에디터나 로그에서 바로 잡을 수 있도록 강하게 크래시를 유도
    checkf(!TargetPointsArray.IsEmpty(), TEXT("No valid target point found in level : %s for spawning enemies"), *GetWorld()->GetName());
    
    // 이번 함수 호출에서 실제로 스폰에 성공한 적 수
    uint32 EnemiesSpawnedThisTime = 0;
    
    // 스폰 파라미터 설정
    FActorSpawnParameters SpawnParam;
    // 충돌 여부와 관계없이 무조건 스폰되도록 설정
    SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    // 현재 웨이브에 대한 스포너 테이블(Row)에 정의된 스포너 정보들을 순회
    for (const FWarriorEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
    {
        // 스폰할 적 클래스(소프트 레퍼런스)가 비어 있으면 건너뛰기
        if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;
       
        // 해당 스포너 정보에서 한 번에 생성할 적의 수를 [최소, 최대] 범위 안에서 랜덤으로 결정
        const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);
       
        // 사전에 로드해둔 소프트 클래스 → 실제 UClass 매핑 테이블에서 클래스 조회
        UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);
       
        // 이번 스포너 정보 기준으로 결정된 수만큼 적 스폰 시도
        for (int32 i = 0; i < NumToSpawn; i++)
        {
            // 사용 가능한 타겟 포인트 중 하나를 랜덤으로 선택
            const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPointsArray.Num() - 1);
            const FVector SpawnOrigin = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
            // 타겟 포인트의 Forward 방향을 그대로 회전 값으로 사용
            const FRotator SpawnRotation = TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();
          
            // 네비 메시 위에서 사용할 랜덤 위치
            FVector RandomLocation;
            // 지정한 타겟 포인트를 중심으로 반경 400 안에서 네비게이션 가능한 랜덤 위치 하나를 선택
            UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnOrigin, RandomLocation, 400.0f);
          
            // 적 캐릭터가 땅에 박히지 않도록 Z축으로 약간 상승
            RandomLocation += FVector(0.0f, 0.0f, 150.0f);
          
            // 실제 적 캐릭터 액터 스폰
            AWarriorEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AWarriorEnemyCharacter>(LoadedEnemyClass, RandomLocation, SpawnRotation, SpawnParam);
          
            // 스폰에 성공했다면 카운터 갱신
            if (SpawnedEnemy)
            {
            	// 적 제거 시 델리게이트 바인딩
            	SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);
                // 이번 함수 호출에서 스폰된 수 증가
                EnemiesSpawnedThisTime++;
                // 이 웨이브 전체 기준 누적 스폰 수 증가
                TotalSpawnedEnemiesThisWaveCounter++;
            }
          
            // 웨이브 설정 상 스폰해야 할 총 적 수를 이미 채웠다면 더 이상 스폰하지 않고 조기 종료
            if (!ShouldKeepSpawnEnemies())
            {
                return EnemiesSpawnedThisTime;
            }
        }
    }
    
    // 모든 스포너 정보를 처리했거나 중간에 더 이상 스폰할 필요가 없어져 루프를 빠져나왔을 때,
    // 이번 호출 동안 실제로 스폰한 적의 수를 반환
    return EnemiesSpawnedThisTime;
}

bool AWarriorSurvivalGameMode::ShouldKeepSpawnEnemies() const
{
	// 현재 웨이브의 목표 스폰 수에 아직 도달하지 않았다면 계속 스폰
    return TotalSpawnedEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
}

void AWarriorSurvivalGameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{
	// 현재 필드에 살아있는 적 수 감소
	CurrentSpawnedEnemiesCounter--;
	
	// 아직 이 웨이브에서 더 스폰해야 할 적이 남아 있다면
	if (ShouldKeepSpawnEnemies())
	{
		// 방금 빈 자리가 난 만큼 추가로 스폰하고, 그 수를 현재 필드 적 수에 더하기
		CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
	}
	// 더 이상 스폰할 적은 없고, 현재 필드에 남은 적도 없다면
	else if (CurrentSpawnedEnemiesCounter == 0)
	{
		// 다음 웨이브를 위해 웨이브 단위 누적 스폰 수와 현재 필드 적 수 초기화
		TotalSpawnedEnemiesThisWaveCounter = 0;
		CurrentSpawnedEnemiesCounter = 0;
       
		// 이 웨이브가 완전히 끝났음을 알리는 상태로 전환
		SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaveCompleted);
	}
}

void AWarriorSurvivalGameMode::RegisterSpawnedEnemies(const TArray<AWarriorEnemyCharacter*>& InEnemiesToRegister)
{
	// 새로 스폰된 적 배열을 순회하면서
	for (AWarriorEnemyCharacter* SpawnedEnemy : InEnemiesToRegister)
	{
		if (SpawnedEnemy)
		{
			// 유효한 적마다 현재 필드에 살아있는 적 수를 1 증가시키고
			CurrentSpawnedEnemiesCounter++;
          
			// 해당 적이 파괴될 때 호출될 델리게이트에 OnEnemyDestroyed를 등록해서
			// 죽을 때마다 카운터 감소 및 웨이브 진행 로직이 동작하도록 연결
			SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);
		}
	}
}
