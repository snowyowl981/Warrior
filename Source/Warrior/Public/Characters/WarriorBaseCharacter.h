// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "WarriorBaseCharacter.generated.h"

class UPawnCombatComponent;

UCLASS()
class WARRIOR_API AWarriorBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IPawnCombatInterface, public IPawnUIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWarriorBaseCharacter();

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface.

	//~ Begin IPawnCombatInterface Interface.
	virtual UPawnCombatComponent* GetCombatComponent() const override;
	//~ End IPawnCombatInterface Interface.

	//~ Begin IPawnUIInterface Interface.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	//~ End IPawnUIInterface Interface.

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface.

	// 어빌리티 시스템 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	class UWarriorAbilitySystemComponent* WarriorAbilitySystemComponent;

	// 게임 플레이 속성
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	class UWarriorAttributeSet* WarriorAttributeSet;

	// 소프트 참조할 캐릭터 시작 데이터
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<class UDataAsset_StartUpDataBase> CharacterStartUpData;

public:
	// get, set의 경우 가독성을 위헤 FORCEINLINE 사용
	FORCEINLINE UWarriorAbilitySystemComponent* GetWarriorAbilitySystemComponent()	const { return WarriorAbilitySystemComponent; }
	FORCEINLINE UWarriorAttributeSet*			GetWarriorAttributeSet()			const { return WarriorAttributeSet; }
};
