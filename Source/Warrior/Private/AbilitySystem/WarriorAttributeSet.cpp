// SnowyOwl All Rights Reserved


#include "AbilitySystem/WarriorAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/HeroUIComponent.h"

#include "WarriorDebugHelper.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	// 생성 시 속성값 초기화
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UWarriorAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	// 캐싱한 PawnUIInterface가 유효하지 않은 경우
	if (!CachedPawnUIInterface.IsValid())
	{
		// AvatarActor에서 IPawnUIInterface 
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}
	// CachedPawnUIInterface 유효성 체크
	checkf(CachedPawnUIInterface.IsValid(), TEXT("%s didn't implement IPawnUIInterface"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());

	// PawnUIComponent 할당 및 유효성 체크
	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	checkf(PawnUIComponent, TEXT("Couldn't Extract a PawnUIComponent from %s"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());
	
	// 속성 값이 현재 체력 가져오기 속성과 같으면 게임플레이 효과를 사용해 현재 체력 속성 수정중이란 의미
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		// 현재 체력 설정 최대값 : 최대 체력, 최소값 : 0
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		// 현재 체력 설정
		SetCurrentHealth(NewCurrentHealth);

		// 최대 체력에 대한 현재 체력 비율 PawnUIComponent에 전달
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}

	// 속성 값이 현재 분노 가져오기 속성과 같으면 게임플레이 효과를 사용해 현재 분노 속성 수정중이란 의미
	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		// 현재 분노 설정 최대값 : 최대 분노, 최소값 : 0
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());

		// 현재 분노 설정
		SetCurrentRage(NewCurrentRage);

		// HeroUIComponent 할당 및 유효성 체크
		if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			// 최대 분노에 대한 현재 분노 비율 PawnUIComponent에 전달
			HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
		}
		
	}

	// 속성 값이 현재 받는 피해량 속성과 같으면 게임플레이 효과를 사용해 현재 받는 피해량 속성을 수정해 피해를 입히고 있다는 의미
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		// 기존 체력
		const float OldHealth = GetCurrentHealth();
		// 받은 대미지
		const float DamageDone = GetDamageTaken();
		// 대미지를 입은 현재 체력
		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

		// 현재 체력 설정
		SetCurrentHealth(NewCurrentHealth);

		// const FString DebugString = FString::Printf(
		// 	TEXT("Old Health : %f, Damage Done : %f, NewCurrentHealth : %f"),
		// 	OldHealth,
		// 	DamageDone,
		// 	NewCurrentHealth
		// );
		//
		// Debug::Print(DebugString, FColor::Red);

		// UI에 알림 표시
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
		
		// 캐릭터 사망 처리 (NewCurrentHealth 사용해도 무관)
		if (GetCurrentHealth() == 0.f)
		{
			// 액터에 사망 상태 태그 추가 (이 액터에 동일한 태그를 여러 번 추가하는 것 방지)
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Shared_Status_Dead);
			
		}
	}
}
