// SnowyOwl All Rights Reserved


#include "Characters/WarriorEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"

#include "WarriorDebugHelper.h"

AWarriorEnemyCharacter::AWarriorEnemyCharacter()
{
	// AI 컨트롤러가 이 캐릭터를 자동으로 조종하도록 설정 (월드에 배치되거나 스폰될 때)
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 컨트롤러의 회전을 캐릭터에 적용하지 않음 (카메라 회전과 무관하게 캐릭터 회전)
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터 이동 설정
	// 컨트롤러가 지정한 회전을 사용하지 않음
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// 이동 방향을 기준으로 자동 회전 (예: 이동하는 방향을 바라봄)
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 회전 속도 설정 (Yaw 회전 속도 0 — 필요 시 변경 가능)
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 0.f);

	// 걷기 속도 설정
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	// 걷기 상태에서의 감속 (브레이크) 설정
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	// EnemyCombatComponent 생성 및 초기화
	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");
}

UPawnCombatComponent* AWarriorEnemyCharacter::GetCombatComponent() const
{
	return EnemyCombatComponent;
}

void AWarriorEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

void AWarriorEnemyCharacter::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull())
	{
		return;
	}

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this]()
			{
				if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
				{
					LoadedData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponent);

					Debug::Print(TEXT("Enemy Start Up Data Loaded"), FColor::Green);
				}
			}
		)	
	);
}
