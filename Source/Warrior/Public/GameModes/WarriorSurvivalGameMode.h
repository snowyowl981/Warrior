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
	// 모든 웨이브 클리어 여부
	bool HasFinishedAllWaves() const;
	// 다음 웨이브 적 미리 비동기 로드
	void PreLoadNextWaveEnemies();
	
	// 현재 웨이브 스포너 테이블 가져오기
	FWarriorEnemyWaveSpawnerTableRow* GetCurrentWaveSpawnerTableRow() const;
	
	// 적 스폰 시도
	int32 TrySpawnWaveEnemies();
	bool ShouldKeepSpawnEnemies() const;
	
	// 현재 ENUM 상태 추적
	UPROPERTY()
	EWarriorSurvivalGameModeState CurrentSurvivalGameModeState;
	
	// 게임모드 상태 변경 시 호출할 델리게이트
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSurvivalGameModeStateChangedDelegate OnSurvivalGameModeStateChanged;
	
	// 에디터에서 생성 및 블루프린트에 할당할 적 웨이브 생성기 데이터 테이블
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemyWaveSpawnerDataTable;
	
	// 총 웨이브 개수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 TotalWavesToSpawn;
	
	// 현재 웨이브
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 CurrentWaveCount = 1;
	
	// 현재 스폰된 적 카운터
	UPROPERTY()
	int32 CurrentSpawnedEnemiesCounter = 0;
	
	// 현재 웨이브에서 스폰시킬 적의 토탈 카운터
	UPROPERTY()
	int32 TotalSpawnedEnemiesThisWaveCounter = 0;
	
	// 타겟 포인트 배열 (스폰 위치)
	UPROPERTY()
	TArray<AActor*> TargetPointsArray;
	
	// 시작 이후 경과된 시간
	UPROPERTY()
	float TimePassedSinceStart = 0.0f;
	
	// 새 웨이브 생성 전 카운트다운
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnNewWaveWaitTime = 5.0f;
	
	// 적 스폰 지연 시간 (위젯이 사라지길 기다렸다가 적을 스폰하는 데 사용)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnEnemiesDelayTime = 2.0f;
	
	// 웨이브 완료 시 대기 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float WaveCompletedWaitTime = 5.0f;
	
	// 비동기 로딩이 끝난 적 캐릭터 클래스를 캐시해 두는 맵
	// Key  : 스폰 대상 적 캐릭터의 소프트 클래스 포인터 (지연 로딩용 레퍼런스)
	// Value: 실제 로드가 완료된 적 캐릭터의 UClass 포인터
	UPROPERTY()
	TMap<TSoftClassPtr<AWarriorEnemyCharacter>, UClass*> PreLoadedEnemyClassMap;
};
