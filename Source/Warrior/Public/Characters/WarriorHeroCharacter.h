// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/WarriorBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "WarriorHeroCharacter.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorHeroCharacter : public AWarriorBaseCharacter
{
	GENERATED_BODY()

public:
	AWarriorHeroCharacter();

	//~ Begin IPawnCombatInterface Interface.
	virtual UPawnCombatComponent* GetCombatComponent() const override;
	//~ End IPawnCombatInterface Interface.

	//~ Begin IPawnUIInterface Interface.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UHeroUIComponent* GetHeroUIComponent() const override;
	//~ End IPawnUIInterface Interface.

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface.\

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

private:
#pragma region Components

	// 스프링 암 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// 영웅 전투 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UHeroCombatComponent* HeroCombatComponent;

	// 영웅 UI 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UHeroUIComponent* HeroUIComponent;
#pragma endregion


#pragma region Inputs
	// 입력 데이터 에셋
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	class UDataAsset_InputConfig* InputConfigDataAsset;

	// 이동 입력 함수
	void Input_Move(const struct FInputActionValue& InputActionValue);
	// 카메라 입력 함수
	void Input_Look(const struct FInputActionValue& InputActionValue);

	// 타겟 변경 입력 함수
	void Input_SwitchTargetTriggered(const struct FInputActionValue& InputActionValue);
	void Input_SwitchTargetCompleted(const struct FInputActionValue& InputActionValue);

	// 방향 전환 FVector 변수 (락온 타겟 변경에 사용)
	FVector2D SwitchDirection = FVector2D::ZeroVector;

	// 어빌리티 인풋 눌렀을 때
	void Input_AbilityInputPressed(FGameplayTag InputTag);
	// 어빌리티 인풋 뗏을 때
	void Input_AbilityInputReleased(FGameplayTag InputTag);
		
#pragma endregion

public:
	FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const { return HeroCombatComponent; }
	
};
