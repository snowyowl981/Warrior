// SnowyOwl All Rights Reserved


#include "Interfaces/PawnUIInterface.h"

// Add default functionality here for any IPawnUIInterface functions that are not pure virtual.
class UHeroUIComponent* IPawnUIInterface::GetHeroUIComponent() const
{
	return nullptr;
}

class UEnemyUIComponent* IPawnUIInterface::GetEnemyUIComponent() const
{
	return nullptr;
}
