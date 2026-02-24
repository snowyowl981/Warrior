// Copyright 2025. SnowyOwl All Rights Reserved.


#include "AbilitySystem/Abilities/HeroGameplayAbility_PickUpStones.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Items/PickUps/WarriorStoneBase.h"

/**
 * 돌 줍기 능력을 담당하는 Gameplay Ability 클래스
 * 플레이어 주변의 박스 트레이스를 통해 돌(AWarriorStoneBase 액터)을 탐지하고 수집한다.
 */

void UHeroGameplayAbility_PickUpStones::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, 
    const FGameplayAbilityActorInfo* ActorInfo, 
    const FGameplayAbilityActivationInfo ActivationInfo, 
    const FGameplayEventData* TriggerEventData
)
{
    // 기본 ActivateAbility 로직 실행 (부모 클래스 처리)
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 
}

void UHeroGameplayAbility_PickUpStones::EndAbility(
    const FGameplayAbilitySpecHandle Handle, 
    const FGameplayAbilityActorInfo* ActorInfo, 
    const FGameplayAbilityActivationInfo ActivationInfo, 
    bool bReplicateEndAbility, 
    bool bWasCancelled
)
{
    // 기본 EndAbility 로직 실행 (부모 클래스 처리)
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

/**
 * 주변의 돌을 탐지하고 CollectedStones 배열에 수집하는 함수
 * 주변에 돌이 하나도 없을 경우 능력을 취소한다.
 */
void UHeroGameplayAbility_PickUpStones::CollectStones()
{
    // 이전에 수집된 돌 목록 초기화
    CollectedStones.Empty();
    
    TArray<FHitResult> TraceHits;
    
    // === 박스 트레이스 수행 ===
    // 캐릭터 발밑 방향으로 BoxTrace를 쏘아 주변 돌 감지
    UKismetSystemLibrary::BoxTraceMultiForObjects(
        GetHeroCharacterFromActorInfo(),                                                // 트레이스를 실행할 월드 컨텍스트
        GetHeroCharacterFromActorInfo()->GetActorLocation(),                            // 시작 위치 (캐릭터 위치)
        GetHeroCharacterFromActorInfo()->GetActorLocation() + 
            -GetHeroCharacterFromActorInfo()->GetActorUpVector() * BoxTraceDistance,    // 끝 위치 (캐릭터 아래쪽)
        TraceBoxSize / 2.0f,                                                            // 박스 크기 (절반)
        (-GetHeroCharacterFromActorInfo()->GetActorUpVector()).ToOrientationRotator(),  // 방향 (바닥 방향)
        StoneTraceChannel,                                                              // 감지할 오브젝트 채널
        false,                                                                          // 복잡한 콜리전 무시
        TArray<AActor*>(),                                                              // 무시할 액터 없음
        bDrawDebugShape ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,         // 디버그 표시 옵션
        TraceHits,                                                                      // 결과 저장 배열
        true                                                                            // 물리 기반 복잡도 체크
    );
    
    // === 감지된 액터 중 돌(AWarriorStoneBase)만 수집 ===
    for (const FHitResult& TraceHit : TraceHits)
    {
        if (AWarriorStoneBase* FoundStone = Cast<AWarriorStoneBase>(TraceHit.GetActor()))
        {
            CollectedStones.AddUnique(FoundStone);
        }
    }
    
    // === 주변에 수집 가능한 돌이 없으면 능력 취소 ===
    if (CollectedStones.IsEmpty())
    {
        CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
    }
}

