// SnowyOwl All Rights Reserved


#include "Characters/WarriorBaseCharacter.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/WarriorAttributeSet.h"

// Sets default values
AWarriorBaseCharacter::AWarriorBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	// AbilitySystemComponent, AttributeSet 서브 객체 생성 후 캐스팅 및 할당
	WarriorAbilitySystemComponent	= CreateDefaultSubobject<UWarriorAbilitySystemComponent>(TEXT("WarriorAbilitySystemComponent"));
	WarriorAttributeSet				= CreateDefaultSubobject<UWarriorAttributeSet>(TEXT("WarriorAttributeSet"));
}

// 어빌리티 시스템 컴포넌트 반환 함수
UAbilitySystemComponent* AWarriorBaseCharacter::GetAbilitySystemComponent() const
{
	return GetWarriorAbilitySystemComponent();
}

// 폰 빙의 함수
void AWarriorBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 어빌리티 컴포넌트 nullptr 체크
	if (WarriorAbilitySystemComponent)
	{
		// OwnerActor, AvatarActor 설정
		WarriorAbilitySystemComponent->InitAbilityActorInfo(this, this);

		// 캐릭터 시작 데이터가 null 이 아닌 경우 해당 지점까지에 이르는 콜스택 생성
		// ensure(!CharacterStartUpData.IsNull());
		// 콜스택 생성 및 지정 메시지 출력
		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forgot to assign start up data to %s"), *GetName());

		// if를 사용한 보험 또한 가능
		/*if (ensure(!CharacterStartUpData.IsNull()))
		{
			
		}*/
	}
}
