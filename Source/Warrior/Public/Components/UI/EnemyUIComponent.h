// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"

class UWarriorWidgetBase;
/**
 * 
 */
UCLASS()
class WARRIOR_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
	
public:
	// 그린 적 위젯 배열 등록
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyDrawnWidget(UWarriorWidgetBase* InWidgetToRegister);
	
	// 그린 적 위젯이 있는 경우 제거
	UFUNCTION(BlueprintCallable)
	void RemoveEnemyDrawnWidgetIfAny();
	
private:
	// 이미 그린 적 위젯 배열
	TArray<UWarriorWidgetBase*> EnemyDrawnWidgets;
	
};
