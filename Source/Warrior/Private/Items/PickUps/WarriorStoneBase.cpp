// Copyright 2025. SnowyOwl All Rights Reserved.


#include "Items/PickUps/WarriorStoneBase.h"
#include "Characters/WarriorHeroCharacter.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "WarriorGameplayTags.h"

void AWarriorStoneBase::Consume(UWarriorAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	// 스톤이 적용할 GameplayEffect 클래스가 반드시 설정되어 있는지 확인 (없으면 에디터/실행 중 크래시)
	check(StoneGameplayEffectClass);
    
	// 설정된 GameplayEffect 클래스의 CDO(Class Default Object)를 가져옴
	UGameplayEffect* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
    
	// 스톤의 GameplayEffect를 해당 어빌리티 시스템 컴포넌트에 자기 자신에게 적용
	AbilitySystemComponent->ApplyGameplayEffectToSelf(
	   EffectCDO,                                  // 적용할 GameplayEffect CDO
	   ApplyLevel,                                 // 이 스톤이 적용될 효과 레벨
	   AbilitySystemComponent->MakeEffectContext() // 효과 적용 시 필요한 컨텍스트 정보 생성
	);
    
	// C++ 처리 후 블루프린트에서 후속 로직을 구현할 수 있도록 이벤트 호출
	BP_OnStoneConsumed();
}

void AWarriorStoneBase::OnPickupCollisionSphereBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// 겹친 액터가 워리어 영웅 캐릭터인지 캐스팅하여 확인
	if (AWarriorHeroCharacter* OverlappedHeroCharacter = Cast<AWarriorHeroCharacter>(OtherActor))
	{
		// 영웅의 ASC에서 '돌 줍기' 어빌리티 태그를 가진 어빌리티를 발동 시도
		OverlappedHeroCharacter->GetWarriorAbilitySystemComponent()->
			TryActivateAbilityByTag(WarriorGameplayTags::Player_Ability_PickUp_Stones);
	}
}

