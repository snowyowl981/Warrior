// SnowyOwl All Rights Reserved


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/WarriorAttributeSet.h"
#include "WarriorGameplayTags.h"

#include "WarriorDebugHelper.h"

// 데미지 계산 시 사용할 Attribute들을 정의하는 구조체
struct FWarriorDamageCapture
{
	// AttackPower 속성 캡처 정의 선언 (매크로 내부적으로 CaptureDefinition 멤버를 생성함)
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	// 생성자에서 실제 CaptureDefinition을 초기화
	FWarriorDamageCapture()
	{
		// 공격자는 Source로부터 AttackPower를 캡처함 (버프 같은 건 고려하지 않음)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, AttackPower, Source, false)
		// 방어자는 Target으로부터 DefensePower를 캡처함
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DefensePower, Target, false)
		// 방어자는 Target으로부터 DamageTaken을 캡처함
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DamageTaken, Target, false)
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

	// AttackPower와 DefensePower, DamageTaken CaptureDef를 목록에 추가
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DamageTakenDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// 이 Effect의 사양(spec)을 가져옴. SetByCaller 값, 캡처된 속성, 메타 정보 등이 들어 있음.
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	
	// 참고: EffectSpec.GetContext()는 다음 정보를 제공함
	// - GetSourceObject(): Ability가 적용된 객체
	// - GetAbility(): 발동한 GameplayAbility 객체
	// - GetInstigator(): 액터를 유발한 주체
	// - GetEffectCauser(): 실제로 대미지를 가한 원인 (무기 등)

	// Attribute 계산 시 필요한 태그 정보 설정
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags(); // 소스(공격자)의 태그
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags(); // 타겟(피격자)의 태그

	// 캡처된 Source의 AttackPower 속성 값을 계산해서 가져옴
	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetWarriorDamageCapture().AttackPowerDef,
		EvaluateParameters,
		SourceAttackPower
	);
	// Debug::Print(TEXT("SourceAttackPower"), SourceAttackPower);

	// SetByCaller로 전달된 커스텀 값들 (기본 대미지, 콤보 수 등)을 읽기 위한 변수들
	float BaseDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;

	// SetByCaller Tag에 따라 각 수치를 분류
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		// 기본 대미지 설정
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
			// Debug::Print(TEXT("BaseDamage"), BaseDamage);
		}

		// 라이트 공격 콤보 횟수
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttackComboCount = TagMagnitude.Value;
			// Debug::Print(TEXT("UsedLightAttackComboCount"), UsedLightAttackComboCount);
		}

		// 헤비 공격 콤보 횟수
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
			// Debug::Print(TEXT("UsedHeavyAttackComboCount"), UsedHeavyAttackComboCount);
		}
	}

	// 타겟의 방어력을 계산
	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetWarriorDamageCapture().DefensePowerDef,
		EvaluateParameters,
		TargetDefensePower
	);
	// Debug::Print(TEXT("TargetDefensePower"), TargetDefensePower);
	
	// 약공격 콤보 카운트를 사용한 기본 공격력 5% 증가
	if (UsedLightAttackComboCount != 0)
	{
		const float DamageIncreasePercentLight = (UsedLightAttackComboCount - 1) * 0.05 + 1.f;

		BaseDamage *= DamageIncreasePercentLight;
		// Debug::Print(TEXT("ScaledBasedDamageLight"), BaseDamage);
	}
	
	// 강공격 콤보 카운트를 사용한 기본 공격력 15% 증가
	if (UsedHeavyAttackComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = UsedHeavyAttackComboCount * 0.15 + 1.f;

		BaseDamage *= DamageIncreasePercentHeavy;
		// Debug::Print(TEXT("ScaledBasedDamageHeavy"), BaseDamage);
	}

	// 최종 피해 계산
	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;
	// Debug::Print(TEXT("FinalDamageDone"), FinalDamageDone);
	
	if (FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetWarriorDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}
