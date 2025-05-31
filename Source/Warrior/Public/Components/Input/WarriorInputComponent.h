// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "WarriorInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// 인풋 액션 바인딩 템플릿
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);

	// 어빌리티 액션 바인딩 템플릿
	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc);
};

// 인풋 액션 바인딩 함수 
template<class UserObject, typename CallbackFunc>
inline void UWarriorInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	// check, checkf 매크로를 통해 입력이 null인 경우를 체크해 에디터 크래시를 방지 (언리얼에서 자주 사용)
	checkf(InInputConfig, TEXT("Input config data asset is null, cannot proceed with binding"));

	// 액션이 유효한지 확인
	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		// 액션 바인딩
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}

// 어빌리티 액션 바인딩 함수
template <class UserObject, typename CallbackFunc>
inline void UWarriorInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
	// check, checkf 매크로를 통해 입력이 null인 경우를 체크해 에디터 크래시를 방지 (언리얼에서 자주 사용)
	checkf(InInputConfig, TEXT("Input config data asset is null, cannot proceed with binding"));

	// 어빌리티 입력 액션 배열 루프
	for (const FWarriorInputActionConfig& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
	{
		// 해당 어빌리티 입력 액션이 유효하지 않으면 다음 루프로
		if (!AbilityInputActionConfig.IsValid()) continue;

		// 눌렀을 때 실행할 함수 바인딩
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started, ContextObject, InputPressedFunc, AbilityInputActionConfig.InputTag);

		// 뗏을 때 실행할 함수 바인딩
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed, ContextObject, InputReleasedFunc, AbilityInputActionConfig.InputTag);
	}
}
