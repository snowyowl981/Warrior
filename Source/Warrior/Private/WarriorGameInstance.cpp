// Copyright 2025. SnowyOwl All Rights Reserved.


#include "WarriorGameInstance.h"
#include "MoviePlayer.h"

// 인스턴스 초기화
void UWarriorGameInstance::Init()
{
	// 부모 GameInstance의 초기화 로직 실행
	Super::Init();
    
	// 맵 로드 직전에 호출되는 델리게이트에 GameInstance의 콜백 등록
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::OnPreLoadMap);
    
	// 맵이 로드 완료되고 월드가 준비됐을 때 호출되는 델리게이트에 콜백 등록
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnDestinationWorldLoaded);
}

void UWarriorGameInstance::OnPreLoadMap(const FString& MapName)
{
	// 로딩 화면 설정을 담는 구조체 생성
	FLoadingScreenAttributes LoadingScreenAttributes;
    
	// 맵 로딩이 끝나면 자동으로 로딩 화면을 닫도록 설정
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
    
	// 로딩이 너무 빨리 끝나도 최소 2초는 로딩 화면을 보여주도록 설정
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.0f;
    
	// 표시할 로딩 위젯 지정 (테스트용 로딩 위젯 생성 함수 사용)
	LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
    
	// 설정한 속성으로 로딩 화면 구성
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}

void UWarriorGameInstance::OnDestinationWorldLoaded(UWorld* LoadedWorld)
{
	// 맵 로드가 완료되었으므로 로딩 화면(무비 플레이어) 종료
	GetMoviePlayer()->StopMovie();
}

TSoftObjectPtr<UWorld> UWarriorGameInstance::GetGameLevelByTag(FGameplayTag InTag) const
{
	// 등록된 게임 레벨 셋들을 순회
	for (const FWarriorGameLevelSet& GameLevelSet : GameLevelSets)
	{
		// 셋 자체가 유효하지 않으면 건너뛰기
		if (!GameLevelSet.IsValid()) continue;
       
		// 전달받은 태그와 일치하는 레벨 태그를 찾은 경우
		if (GameLevelSet.LevelTag == InTag)
		{
			// 해당 셋에 설정된 레벨(월드) 소프트 레퍼런스를 반환
			return GameLevelSet.Level;
		}
	}
    
	// 일치하는 태그를 가진 레벨을 찾지 못한 경우, 비어 있는 소프트 레퍼런스를 반환
	return TSoftObjectPtr<UWorld>();
}
