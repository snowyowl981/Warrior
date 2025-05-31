// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorFunctionLibrary.generated.h"

// Forward declaration of the custom AbilitySystemComponent
class UWarriorAbilitySystemComponent;

// 사용자 정의 Enum - 확인 타입 (블루프린트에서 사용 가능)
UENUM()
enum class EWarriorConfirmType : uint8
{
	Yes, // 확인됨
	No   // 확인되지 않음
};

/**
 * 유틸리티 함수들을 포함한 정적 함수 라이브러리 클래스입니다.
 * 블루프린트에서도 호출 가능하며, 주로 Actor와 GameplayTag 관련 기능을 제공합니다.
 */
UCLASS()
class WARRIOR_API UWarriorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// 주어진 Actor로부터 WarriorAbilitySystemComponent를 반환 (C++ 전용)
	static UWarriorAbilitySystemComponent* NativeWarriorASCFromActor(AActor* InActor);

	/**
	 * Actor에 특정 GameplayTag가 존재하지 않으면 추가합니다.
	 *
	 * @param InActor		대상 Actor
	 * @param TagToAdd		추가할 GameplayTag
	 */
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	/**
	 * Actor에 특정 GameplayTag가 존재하면 제거합니다.
	 *
	 * @param InActor		대상 Actor
	 * @param TagToRemove	제거할 GameplayTag
	 */
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void RemoveGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	// Actor가 특정 GameplayTag를 가지고 있는지 확인 (C++ 전용)
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	/**
	 * Actor가 특정 GameplayTag를 가지고 있는지 블루프린트에서 확인합니다.
	 * 결과는 Enum Exec 핀으로 반환됩니다.
	 *
	 * @param InActor			대상 Actor
	 * @param TagToCheck		확인할 GameplayTag
	 * @param OutConfirmType	결과 (Yes or No)
	 */
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType);
};
