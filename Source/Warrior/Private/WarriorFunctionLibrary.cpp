// SnowyOwl All Rights Reserved


#include "WarriorFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"

// Actor로부터 Warrior 전용 AbilitySystemComponent를 가져옵니다.
// 반드시 존재해야 하므로 유효성 검사(check)를 수행하고, 실패 시 에디터에서 크래시가 발생합니다.
UWarriorAbilitySystemComponent* UWarriorFunctionLibrary::NativeWarriorASCFromActor(AActor* InActor)
{
	check(InActor); // 유효하지 않은 Actor이면 여기서 중단됨 (디버그용 강제 검사)

	// AbilitySystemComponent를 가져와서 Warrior 전용 컴포넌트로 캐스팅
	return CastChecked<UWarriorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

// Actor에 GameplayTag가 없을 경우에만 해당 Tag를 추가합니다.
void UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UWarriorAbilitySystemComponent* ASC = NativeWarriorASCFromActor(InActor);

	// 지정된 태그가 없다면 추가
	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd); // 일시적인 태그 추가 (서버/클라 동기화 X)
	}
}

// Actor가 해당 GameplayTag를 가지고 있을 경우에만 제거합니다.
void UWarriorFunctionLibrary::RemoveGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UWarriorAbilitySystemComponent* ASC = NativeWarriorASCFromActor(InActor);

	// 지정된 태그가 존재하면 제거
	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove); // 일시적인 태그 제거
	}
}

// Actor가 해당 GameplayTag를 가지고 있는지 여부를 반환합니다. (C++ 전용)
bool UWarriorFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UWarriorAbilitySystemComponent* ASC = NativeWarriorASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck); // 태그 보유 여부 반환
}

// 블루프린트 전용 함수: Actor가 특정 태그를 가지고 있는지 확인 후, Enum(Yes/No) 형태로 반환합니다.
void UWarriorFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType)
{
	// Native C++ 함수 호출 결과를 기반으로 Enum 값 설정
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EWarriorConfirmType::Yes : EWarriorConfirmType::No;
}
