// Copyright 2025. SnowyOwl All Rights Reserved.


#include "GameModes/WarriorSurvivalGameMode.h"

#include "AnimNodes/AnimNode_RandomPlayer.h"

void AWarriorSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// 적 웨이브 스포너 데이터 테이블 확인
	checkf(EnemyWaveSpawnerDataTable, TEXT("Forgot to assign a valid data table"));
	
	// 게임모드 상태 설정 (새 웨이브 스폰 대기중)
	SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);
	
	// 데이터 테이블로부터 총 웨이브 수 할당
	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();
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
			
			// 모든 웨이브 끝낸 경우, 아닌 경우 게임모드 상태 설정
			if (HasFinishedAllWaves())
			{
				SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::AllWavesDone);
			}
			else
			{
				SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);
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
