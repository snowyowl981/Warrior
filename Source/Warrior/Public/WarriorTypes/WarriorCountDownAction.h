// Copyright 2025. SnowyOwl All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WarriorTypes/WarriorEnumTypes.h"

// ======================================
// FWarriorCountDownAction
// --------------------------------------
// 이 클래스는 일정 시간 동안 카운트다운을 진행하는 Latent Action
// (예: 블루프린트의 Delay나 Wait 같은 방식으로 동작)
// ======================================

class FWarriorCountDownAction : public FPendingLatentAction
{
public:
    // --------------------------------------------------
    // 생성자
    // InTotalCountDownTime : 전체 카운트다운 시간(초)
    // InUpdateInterval     : 매번 업데이트할 주기(초)
    // InOutRemainingTime   : 남은 시간(레퍼런스로 외부에 전달)
    // InCountDownOutput    : 카운트다운 결과 상태(성공, 취소 등)
    // LatentInfo           : 언리얼 latent action 시스템에서 사용되는 정보
    // --------------------------------------------------
    FWarriorCountDownAction(float InTotalCountDownTime, float InUpdateInterval, float& InOutRemainingTime, EWarriorCountDownActionOutput& InCountDownOutput, const FLatentActionInfo& LatentInfo)
        : bNeedToCancel(false)                              // 초기에는 취소 필요 없음
        , TotalCountDownTime(InTotalCountDownTime)          // 총 카운트다운 시간
        , UpdateInterval(InUpdateInterval)                  // 매 업데이트 간격
        , OutRemainingTime(InOutRemainingTime)              // 외부로 전달할 남은 시간 (참조 변수)
        , CountDownOutput(InCountDownOutput)                // 카운트다운 결과 출력 (참조 변수)
        , ExecutionFunction(LatentInfo.ExecutionFunction)   // 실행할 함수 이름 (Latent 시스템 이용)
        , OutputLink(LatentInfo.Linkage)                    // 연결된 latent 노드 링크
        , CallbackTarget(LatentInfo.CallbackTarget)      // 이 액션을 수행 중인 객체
        , ElapsedInterval(0.0f)                             // 마지막 업데이트 이후 경과 시간
        , ElapsedTimeSinceStart(0.0f)                       // 액션 시작 이후 전체 경과 시간
    {
    }

    // --------------------------------------------------
    // Latent Action이 매 프레임(틱)마다 호출하는 함수
    // 내부에서 시간이 지났는지 확인하고, 카운트다운 진행.
    // 카운트다운이 끝나면 Response.Done()으로 완료 처리.
    // --------------------------------------------------
    virtual void UpdateOperation(FLatentResponse& Response) override;

    // --------------------------------------------------
    // 카운트다운을 강제로 중단하고 취소 상태로 설정
    // 외부에서 호출 가능.
    // --------------------------------------------------
    void CancelAction();
    
private:
    // --------------------------------------------------
    // 내부 상태 변수
    // --------------------------------------------------
    bool   bNeedToCancel;                               // true면 액션 취소 예정
    float  TotalCountDownTime;                          // 카운트다운 전체 시간
    float  UpdateInterval;                              // 업데이트 간격
    float& OutRemainingTime;                            // 남은 시간을 반환할 참조 변수
    EWarriorCountDownActionOutput& CountDownOutput;     // 결과를 저장할 참조 변수 (예: 완료, 취소)
    FName  ExecutionFunction;                           // 완료 시 호출할 함수 이름
    int32  OutputLink;                                  // Latent 링크 인덱스
    FWeakObjectPtr CallbackTarget;                      // 콜백을 받을 대상 객체 (일반적으로 UBlueprintAsyncActionBase 등)
    float  ElapsedInterval;                             // 마지막 업데이트 이후 경과 시간
    float  ElapsedTimeSinceStart;                       // 액션 시작 이후 전체 경과 시간
};