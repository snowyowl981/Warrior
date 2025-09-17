// SnowyOwl All Rights Reserved


#include "WarriorFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"

// Actor로부터 Warrior 전용 AbilitySystemComponent를 가져오기
// 반드시 존재해야 하므로 유효성 검사(check)를 수행하고, 실패 시 에디터에서 크래시가 발생
UWarriorAbilitySystemComponent* UWarriorFunctionLibrary::NativeWarriorASCFromActor(AActor* InActor)
{
	check(InActor); // 유효하지 않은 Actor이면 여기서 중단됨 (디버그용 강제 검사)

	// AbilitySystemComponent를 가져와서 Warrior 전용 컴포넌트로 캐스팅
	return CastChecked<UWarriorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

// Actor에 GameplayTag가 없을 경우에만 해당 Tag를 추가
void UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UWarriorAbilitySystemComponent* ASC = NativeWarriorASCFromActor(InActor);

	// 지정된 태그가 없다면 추가
	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd); // 일시적인 태그 추가 (서버/클라 동기화 X)
	}
}

// Actor가 해당 GameplayTag를 가지고 있을 경우에만 제거
void UWarriorFunctionLibrary::RemoveGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UWarriorAbilitySystemComponent* ASC = NativeWarriorASCFromActor(InActor);

	// 지정된 태그가 존재하면 제거
	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove); // 일시적인 태그 제거
	}
}

// Actor가 해당 GameplayTag를 가지고 있는지 여부를 반환 (C++ 전용)
bool UWarriorFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UWarriorAbilitySystemComponent* ASC = NativeWarriorASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck); // 태그 보유 여부 반환
}

// 블루프린트 전용 함수: Actor가 특정 태그를 가지고 있는지 확인 후, Enum(Yes/No) 형태로 반환
void UWarriorFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType)
{
	// Native C++ 함수 호출 결과를 기반으로 Enum 값 설정
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EWarriorConfirmType::Yes : EWarriorConfirmType::No;
}

UPawnCombatComponent* UWarriorFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	// 액터가 IPawnCombatInterface를 구현했다면, 해당 인터페이스로 캐스팅 시도.
	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		// 인터페이스에서 전투 컴포넌트를 가져와 반환.
		return PawnCombatInterface->GetCombatComponent();
	}

	// 캐스팅 되지 않으면 nullptr 반환
	return nullptr;
}

UPawnCombatComponent* UWarriorFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType)
{
	// Native 함수를 통해 전투 컴포넌트 시도 획득.
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

	// 전투 컴포넌트가 유효한 경우 Valid, 그렇지 않으면 Invalid로 결과 설정.
	OutValidType = CombatComponent ? EWarriorValidType::Valid : EWarriorValidType::Invalid;

	// 전투 컴포넌트를 반환.
	return CombatComponent;
}

bool UWarriorFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	// 유효성 체크
	check(QueryPawn && TargetPawn);

	// 팀 에이전트 인터페이스 할당
	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	// 유효성 체크 및 팀ID 같은지 확인
	if (QueryTeamAgent && TargetTeamAgent)
	{
		// 같이 않으면 적대적
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
	// 같으면 우호적
	return false;
}
