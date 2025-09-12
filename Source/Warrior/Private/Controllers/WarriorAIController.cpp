// SnowyOwl All Rights Reserved


#include "Controllers/WarriorAIController.h"
#include "Navigation/CrowdFollowingComponent.h"

#include "WarriorDebugHelper.h"

// AWarriorAIController 클래스의 생성자
AWarriorAIController::AWarriorAIController(const FObjectInitializer& ObjectInitializer)
	// 부모 클래스(Super)의 생성자를 호출하면서, 기본 PathFollowingComponent를 UCrowdFollowingComponent로 교체하도록 설정
	// 이를 통해 이 AI는 다른 AI들을 인식하고 피해가는 '군중 회피' 기능을 사용 가능
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	// 방금 설정한 PathFollowingComponent가 실제로 UCrowdFollowingComponent인지 확인 및 형변환
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		Debug::Print(TEXT("CrowdFollowingComponent valid"), FColor::Green);
	}
}
