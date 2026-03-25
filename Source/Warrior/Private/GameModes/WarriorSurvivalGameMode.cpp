// Copyright 2025. SnowyOwl All Rights Reserved.


#include "GameModes/WarriorSurvivalGameMode.h"

void AWarriorSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AWarriorSurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWarriorSurvivalGameMode::SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState InState)
{
	// 현재 상태 설정
	CurrentSurvivalGameModeState = InState;
	// 델리게이트 브로드캐스트
	OnSurvivalGameModeStateChanged.Broadcast(CurrentSurvivalGameModeState);
}
