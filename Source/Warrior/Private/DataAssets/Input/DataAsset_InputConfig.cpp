// SnowyOwl All Rights Reserved


#include "DataAssets/Input/DataAsset_InputConfig.h"

// FGameplayTag를 기반으로 NativeInputActions 를 찾아 반환하는 함수
UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& IntInputTag) const
{
    // NativeInputActions 배열 순회
    for (const FWarriorInputActionConfig& InputActionConfig : NativeInputActions)
    {
        // 입력 태그가 InputTag가 IntInputTag와 동일하고, InputAction이 nullptr이 아닌 경우
        if (InputActionConfig.InputTag == IntInputTag && InputActionConfig.InputAction)
        {
            // 해당 인풋 액션 반환
            return InputActionConfig.InputAction;
        }
    }

    return nullptr;
}
