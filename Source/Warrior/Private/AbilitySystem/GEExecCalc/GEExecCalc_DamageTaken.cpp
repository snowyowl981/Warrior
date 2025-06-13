// SnowyOwl All Rights Reserved


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/WarriorAttributeSet.h"

// 데미지 계산 시 사용할 Attribute들을 정의하는 구조체
struct FWarriorDamageCapture
{
	// AttackPower 속성 캡처 정의 선언 (매크로 내부적으로 CaptureDefinition 멤버를 생성함)
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)

	// 생성자에서 실제 CaptureDefinition을 초기화
	FWarriorDamageCapture()
	{
		// 공격자는 Source로부터 AttackPower를 캡처함 (버프 같은 건 고려하지 않음)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, AttackPower, Source, false)
		// 방어자는 Target으로부터 DefensePower를 캡처함
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DefensePower, Target, false)
	}
};

// 정적 함수로 Capture 정의 객체를 반환 (정적 지역 변수라 한 번만 초기화됨)
static const FWarriorDamageCapture GetWarriorDamageCapture()
{
	static FWarriorDamageCapture WarriorDamageCapture;
	return WarriorDamageCapture;
}

// Damage 계산용 Execution Calculation 클래스 생성자
UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	/* 느린 방법: 리플렉션을 이용해서 속성을 수동으로 찾아서 CaptureDefinition 생성 */

	// UWarriorAttributeSet 클래스에서 AttackPower 프로퍼티를 찾음
	// FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
	// 	UWarriorAttributeSet::StaticClass(),
	// 	GET_MEMBER_NAME_CHECKED(UWarriorAttributeSet, AttackPower)
	// );
	//
	// // AttackPower 속성의 Capture 정의 수동 생성 (Source에서 캡처, 버프 고려 안 함)
	// FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
	// 	AttackPowerProperty,
	// 	EGameplayEffectAttributeCaptureSource::Source,
	// 	false
	// );
	//
	// // 캡처할 속성 목록에 추가
	// RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);

	/* 빠른 방법: 미리 정의해둔 CaptureDef 사용 */

	// AttackPower와 DefensePower CaptureDef를 목록에 추가
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DefensePowerDef);
}
