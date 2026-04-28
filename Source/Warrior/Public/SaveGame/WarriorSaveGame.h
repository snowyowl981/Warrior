// Copyright 2025. SnowyOwl All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "WarriorSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	// 현재 저장된 게임 난이도
	UPROPERTY(BlueprintReadOnly)
	EWarriorGameDifficulty SavedCurrentGameDifficulty;
};
