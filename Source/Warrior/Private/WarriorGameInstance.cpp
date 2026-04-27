// Copyright 2025. SnowyOwl All Rights Reserved.


#include "WarriorGameInstance.h"

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
