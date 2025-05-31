// SnowyOwl All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARRIOR_API UPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	// 소유 중인 Pawn을 특정 타입으로 반환하는 템플릿 함수
	template <class T>
	T* GetOwningPawn() const
	{
		// T는 반드시 APawn을 상속받은 타입이어야 함
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' Template Parameter for GetPawn must be derived from APawn");

		// 오브젝트의 Owner를 T 타입으로 안전하게 캐스팅하여 반환
		return CastChecked<T>(GetOwner());
	}

	// 기본 Pawn 타입(APawn)으로 반환하는 오버로드 함수
	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	// 소유 중인 Controller를 특정 타입으로 반환하는 템플릿 함수
	template <class T>
	T* GetOwningController() const
	{
		// T는 반드시 AController를 상속받은 타입이어야 함
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' Template Parameter for GetController must be derived from AController");

		// 소유 중인 Pawn을 얻은 뒤, 해당 Pawn으로부터 Controller를 특정 타입으로 반환
		return GetOwningPawn<APawn>()->GetController<T>();
	}
};
