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