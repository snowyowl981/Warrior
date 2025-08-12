// SnowyOwl All Rights Reserved


#include "Widgets/WarriorWidgetBase.h"

#include "Components/UI/EnemyUIComponent.h"
#include "Interfaces/PawnUIInterface.h"

void UWarriorWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UHeroUIComponent* HeroUIComponent = PawnUIInterface->GetHeroUIComponent())
		{
			BP_OnOwningHeroUIComponentInitialized(HeroUIComponent);
		}
	}
}

void UWarriorWidgetBase::InitEnemyCreateWidget(AActor* OwingEnemyActor)
{
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(OwingEnemyActor))
	{
		UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();

		checkf(EnemyUIComponent, TEXT("Failed to Extract an EnemyUIComponent from %s"), *OwingEnemyActor->GetActorNameOrLabel());

	 	BP_OnOwningEnemyUIComponentInitialized(EnemyUIComponent);
	}
}
