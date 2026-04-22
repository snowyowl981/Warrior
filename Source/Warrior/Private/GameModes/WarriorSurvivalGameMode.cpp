// Copyright 2025. SnowyOwl All Rights Reserved.


#include "GameModes/WarriorSurvivalGameMode.h"
#include "Engine/AssetManager.h"
#include "Characters/WarriorEnemyCharacter.h"

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
			// TODO : Handle Spawn New Enemies
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
						Debug::Print(LoadedEnemyClass->GetName() + TEXT(" Is Loaded"));
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
