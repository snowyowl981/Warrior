// Copyright 2025. SnowyOwl All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/WarriorBaseGameMode.h"
#include "WarriorSurvivalGameMode.generated.h"

class AWarriorEnemyCharacter;

// 서바이벌 게임모드 상태 UENUM
UENUM(BlueprintType)
enum class EWarriorSurvivalGameModeState : uint8
{
	WaitSpawnNewWave,	// 새 웨이브 대기
	SpawningNewWave,	// 새 웨이브 스폰중
	InProgress,			// 진행중
	WaveCompleted,		// 웨이브 완료
	AllWavesDone,		// 모든 웨이브 완료
	PlayerDied			// 플레이어 사망
};

// 서바이벌 게임모드 구조체
USTRUCT(BlueprintType)
struct FWarriorEnemyWaveSpawnerInfo
{
	GENERATED_BODY()
	
	// 스폰할 적 클래스 (비동기)
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn;
	
	// 최소 스폰 카운트
	UPROPERTY(EditAnywhere)
	int32 MinPerSpawnCount = 1;
	
	// 최대 스폰 카운트
	UPROPERTY(EditAnywhere)
	int32 MaxPerSpawnCount = 3;
};

// 적 스폰 테이블 구조체
USTRUCT(BlueprintType)
struct FWarriorEnemyWaveSpawnerTableRow : public FTableRowBase
{
	// 본문 매크로
	GENERATED_BODY()
	
	// 적 웨이브 생성기
	UPROPERTY(EditAnywhere)
	TArray<FWarriorEnemyWaveSpawnerInfo> EnemyWaveSpawnerDefinitions;
	
	// 이 웨이브에 스폰할 총 적의 수
	UPROPERTY(EditAnywhere)
	int32 TotalEnemyToSpawnThisWave = 1;
};

// 게임모드 상태 변경 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvivalGameModeStateChangedDelegate, EWarriorSurvivalGameModeState, CurrentState);

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorSurvivalGameMode : public AWarriorBaseGameMode
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
private:
	// 게임모드 상태 Setter
	void SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState InState);
	
	// 현재 ENUM 상태 추적
	UPROPERTY()
	EWarriorSurvivalGameModeState CurrentSurvivalGameModeState;
	
	// 게임모드 상태 변경 시 호출할 델리게이트
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSurvivalGameModeStateChangedDelegate OnSurvivalGameModeStateChanged;
	
	// 에디터에서 생성 및 블루프린트에 할당할 적 웨이브 생성기 데이터 테이블
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemyWaveSpawnerDataTable;
};
