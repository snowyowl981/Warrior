// SnowyOwl All Rights Reserved


#include "Controllers/WarriorAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

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

	// AISenseConfig_Sight 생성 및 초기화
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;		// 적 감지 활성화
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;	// 아군 감지 비활성화
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;	// 중립 감지 비활성화
	
	// 시야 설정
	AISenseConfig_Sight->SightRadius = 5000.f;								// 시야 범위
	AISenseConfig_Sight->LoseSightRadius = 0.f;								// 대상 탐지 최대 시야 범위
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f;				// 탐지 시야각

	// EnemyPerceptionComponent 생성 및 초기화
	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	
	// 시각 감지 설정값을 PerceptionComponent에 등록
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	
	// 지배적인 감지 방식(우선순위 Sense)을 시각(Sight)으로 설정
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	
	// 타겟 인식 상태가 바뀔 때 호출될 델리게이트 바인딩 (OnEnemyPerceptionUpdated 함수 실행)
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	// TeamId 설정
	SetGenericTeamId(FGenericTeamId(1));
}

// GetTeamAttitude 인터페이스 함수
ETeamAttitude::Type AWarriorAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	// 체크할 폰, 팀 에이전트 인터페이스 캐스팅
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());

	// TeamId가 다른 경우 적대, 같은 경우 우호
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

// 타겟 인식 상태가 바뀔 때 호출될 델리게이트
void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// 타겟 감지 시
	if (Stimulus.WasSuccessfullySensed() && Actor)
	{
		// 블랙보드 컴포넌트 할당 및 if 체크
		if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
		{
			// 적 인식 이용해 블랙보드 키 값 설정
			BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
		}
	}
}
