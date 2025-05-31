// SnowyOwl All Rights Reserved


#include "AnimInstances/Hero/WarriorHeroLinkedAnimLayer.h"
#include "AnimInstances/Hero/WarriorHeroAnimInstance.h"

class UWarriorHeroAnimInstance* UWarriorHeroLinkedAnimLayer::GetHeroAnimInstance() const
{
	 return Cast<UWarriorHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
