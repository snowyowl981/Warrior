// SnowyOwl All Rights Reserved


#include "AI/BTService_OrientToTargetActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"

#include "WarriorDebugHelper.h"

UBTService_OrientToTargetActor::UBTService_OrientToTargetActor()
{
	// 노드 이름
	NodeName = TEXT("Native Orient Rotation To Target Actor");

	// 초기화 서비스 노드
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	// 부모 클래스 변수 설정
	RotationInterpSpeed = 5.f;	// 회전 보간 속도 설정
	Interval = 0.f;				// 모든 프레임 실행에 필요
	RandomDeviation = 0.f;		// 무작위 편차 설정

	// 객체 필터 추가 및 드롭다운 선택 키 유형 제한 (상위 클래스에서 함수 재설정해야 사용 가능)
	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

void UBTService_OrientToTargetActor::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	// 블랙보드 에셋 로컬 할당 및 확인
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		// 선택한 키의 Id와 클래스 찾기
		InTargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}

// BTService의 설명 텍스트를 반환하는 함수
FString UBTService_OrientToTargetActor::GetStaticDescription() const
{
	// 블랙보드 키 이름을 문자열로 변환
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();

	// 최종 설명 텍스트를 구성해서 반환
	return FString::Printf(TEXT("Orient Rotation to %s Key %s"), *KeyDescription, *GetStaticServiceDescription());
}

void UBTService_OrientToTargetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// 부모 클래스의 TickNode 호출 (기본 동작 유지)
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 블랙보드에서 대상 액터(Object) 가져오기
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	// 현재 AI 컨트롤러가 조종 중인 Pawn 가져오기
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	// 대상 액터와 자신의 Pawn이 모두 존재할 때만 실행
	if (TargetActor && OwningPawn)
	{
		// 현재 위치에서 대상 위치를 바라보는 회전값 계산
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
			OwningPawn->GetActorLocation(),
			TargetActor->GetActorLocation()
		);

		// 보간(Interp)으로 부드럽게 회전값 변화
		const FRotator TargetRotation = FMath::RInterpTo(
			OwningPawn->GetActorRotation(), // 현재 회전
			LookAtRotation,                 // 목표 회전
			DeltaSeconds,                   // 프레임 시간
			RotationInterpSpeed              // 보간 속도
		);

		// 최종 회전값 적용
		OwningPawn->SetActorRotation(TargetRotation);
	}
}
