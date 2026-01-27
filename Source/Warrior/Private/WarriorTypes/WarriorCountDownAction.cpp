// Copyright 2025. SnowyOwl All Rights Reserved.


#include "WarriorTypes/WarriorCountDownAction.h"


// ======================================================
// FWarriorCountDownAction::UpdateOperation
// ------------------------------------------------------
// 매 프레임마다 호출되어 카운트다운의 상태를 갱신
// 카운트다운이 완료되거나 취소될 경우 Latent Action을 종료,
// 그렇지 않으면 주기(UpdateInterval)에 맞춰 남은 시간을 업데이트
// ======================================================
void FWarriorCountDownAction::UpdateOperation(FLatentResponse& Response)
{
    // --------------------------------------------------
    // [1] 취소 요청이 들어온 경우
    // bNeedToCancel 플래그가 true라면 즉시 액션 중단
    // --------------------------------------------------
    if (bNeedToCancel)
    {
        CountDownOutput = EWarriorCountDownActionOutput::Cancelled; // 결과 : 취소됨
        
        // Latent Action 종료 및 콜백 트리거
        Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
        return;
    }

    // --------------------------------------------------
    // [2] 전체 카운트다운이 완료된 경우
    // (누적된 시간이 총 시간 이상일 때)
    // --------------------------------------------------
    if (ElapsedTimeSinceStart >= TotalCountDownTime)
    {
        CountDownOutput = EWarriorCountDownActionOutput::Completed; // 결과 : 완료됨
        
        // Latent Action 종료 및 콜백 트리거
        Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
        return;
    }

    // --------------------------------------------------
    // [3] 아직 UpdateInterval(업데이트 주기)이 지나지 않은 경우
    // -> 시간 누적만 진행
    // --------------------------------------------------
    if (ElapsedInterval < UpdateInterval)
    {
        // 이번 프레임에서 지난 시간만큼 누적
        ElapsedInterval += Response.ElapsedTime();
    }
    else
    {
        // --------------------------------------------------
        // [4] UpdateInterval이 지난 경우 → 카운트다운 갱신 수행
        // --------------------------------------------------
        
        // 총 경과 시간 누적
        ElapsedTimeSinceStart += UpdateInterval > 0.0f ? UpdateInterval : Response.ElapsedTime();

        // 남은 시간 계산 (외부로 전달할 참조 변수)
        OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;

        // 갱신 상태 전달 (Updated 상태를 외부로 알림)
        CountDownOutput = EWarriorCountDownActionOutput::Updated;

        // 연결된 Latent 노드(예: Blueprint의 “Update” 핀 등) 트리거 실행
        Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);

        // Interval 누적 시간 초기화 (다음 주기 대기용)
        ElapsedInterval = 0.0f;
    }
}

// ======================================================
// FWarriorCountDownAction::CancelAction
// ------------------------------------------------------
// 외부에서 호출해 카운트다운을 강제로 취소.
// 다음 Tick에서 bNeedToCancel 확인 후 즉시 종료.
// ======================================================
void FWarriorCountDownAction::CancelAction()
{
    bNeedToCancel = true;
}

