// SnowyOwl All Rights Reserved


#include "Components/UI/EnemyUIComponent.h"
#include "Widgets/WarriorWidgetBase.h"

void UEnemyUIComponent::RegisterEnemyDrawnWidget(UWarriorWidgetBase* InWidgetToRegister)
{
	// 등록할 적 위젯을 배열에 추가
	EnemyDrawnWidgets.Add(InWidgetToRegister);
}

void UEnemyUIComponent::RemoveEnemyDrawnWidgetIfAny()
{
	// 배열이 비어있는 경우 함수 종료
	if (EnemyDrawnWidgets.IsEmpty())
	{
		return;
	}
	
	// 배열 돌면서 생성된 위젯 제거
	for (UWarriorWidgetBase* DrawnWidget : EnemyDrawnWidgets)
	{
		if (DrawnWidget)
		{
			DrawnWidget->RemoveFromParent();
		}
	}
	
	// 배열 비우기
	EnemyDrawnWidgets.Empty();
}
