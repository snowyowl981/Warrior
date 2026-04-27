// Copyright 2025. SnowyOwl All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "WarriorGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FWarriorGameLevelSet
{
	GENERATED_BODY()
	
	// 레벨 태그
	UPROPERTY(EditDefaultsOnly, meta=(Categories = "GameData.Level"))
	FGameplayTag LevelTag;
	
	// 레벨 소프트 오브젝트 포인터
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;
	
	// 유효성 확인 헬퍼 함수
	bool IsValid() const
	{
		// 레벨 태그와 레벨이 유효한지 확인해 반환
		return LevelTag.IsValid() && !Level.IsNull();
	}
};

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	// 게임 레벨 배열
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWarriorGameLevelSet> GameLevelSets;
	
public:
	// 태그별로 게임 레벨 로드
	UFUNCTION(BlueprintPure, meta = (GameplayTagFilter = "GameData.Level"))
	TSoftObjectPtr<UWorld> GetGameLevelByTag(FGameplayTag InTag) const;
};
