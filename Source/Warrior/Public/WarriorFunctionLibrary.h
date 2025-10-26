// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "WarriorFunctionLibrary.generated.h"

// Forward declaration
class UWarriorAbilitySystemComponent;
class UPawnCombatComponent;

struct FScalableFloat;

/**
 * 유틸리티 함수들을 포함한 정적 함수 라이브러리 클래스
 * 블루프린트에서도 호출 가능하며, 주로 Actor와 GameplayTag 관련 기능을 제공
 */
UCLASS()
class WARRIOR_API UWarriorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// 주어진 Actor로부터 WarriorAbilitySystemComponent를 반환 (C++ 전용)
	static UWarriorAbilitySystemComponent* NativeWarriorASCFromActor(AActor* InActor);

	/**
	 * Actor에 특정 GameplayTag가 존재하지 않으면 추가
	 *
	 * @param InActor		대상 Actor
	 * @param TagToAdd		추가할 GameplayTag
	 */
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	/**
	 * Actor에 특정 GameplayTag가 존재하면 제거
	 *
	 * @param InActor		대상 Actor
	 * @param TagToRemove	제거할 GameplayTag
	 */
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void RemoveGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	// Actor가 특정 GameplayTag를 가지고 있는지 확인 (C++ 전용)
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	/**
	 * Actor가 특정 GameplayTag를 가지고 있는지 블루프린트에서 확인
	 * 결과는 Enum Exec 핀으로 반환
	 *
	 * @param InActor			대상 Actor
	 * @param TagToCheck		확인할 GameplayTag
	 * @param OutConfirmType	결과 (Yes or No)
	 */
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType);

	// Actor로부터 CombatComponent를 가져오는 함수
	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);


	/**
	 * Blueprint에서 사용할 수 있는 함수: Native 함수 호출 후, 결과 유효성도 함께 반환.
	 *
	 * @param InActor			대상 Actor
	 * @param OutValidType		결과 (Valid or Invalid)
	 */
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (DisplayName = "Get PawnCombatComponent From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType);

	// 타겟 폰 적대 확인
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

	// 레벨에 따른 ScalableFloat 가져오기
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary", meta=(CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel = 1.f);

	// HitReact 방향
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference);
};
