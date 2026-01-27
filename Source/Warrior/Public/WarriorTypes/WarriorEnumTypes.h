#pragma once

// 사용자 정의 Enum - 확인 타입 (블루프린트에서 사용 가능)
UENUM()
enum class EWarriorConfirmType : uint8
{
	Yes,		// 확인됨
	No			// 확인되지 않음
};

// 사용자 정의 Enum - 유효 타입 (블루프린트에서 사용 가능)
UENUM()
enum class EWarriorValidType : uint8
{
	Valid,		// 유효함
	Invalid		// 유효하지 않음
};

// 사용자 정의 Enum - 성공 타입 (블루프린트에서 사용 가능)
UENUM()
enum class EWarriorSuccessType : uint8
{
	Successful,	// 성공
	Failed		// 실패
};

// 사용자 정의 Enum - 카운트다운 액션 입력 (블루프린트에서 사용 가능)
UENUM()
enum class EWarriorCountDownActionInput : uint8
{
	Start,		// 시작
	Cancel		// 취소
};

// 사용자 정의 Enum - 카운트다운 액션 출력 (블루프린트에서 사용 가능)
UENUM()
enum class EWarriorCountDownActionOutput : uint8
{
	Updated,		// 업데이트됨
	Completed,		// 완료됨
	Cancelled		// 취소됨
};