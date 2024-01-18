// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(CustomLog, Log, All)
//DECLARE_LOG_CATEGORY_EXTERN(BWarning, Warning, All)
//DECLARE_LOG_CATEGORY_EXTERN(BError, Error, All)
//DECLARE_LOG_CATEGORY_EXTERN(BFatal, Fatal, All)
//DECLARE_LOG_CATEGORY_EXTERN(BVerbose, Verbose, All)
//DECLARE_LOG_CATEGORY_EXTERN(BVeryVerbose, VeryVerbose, All)

#define LINEINFO					  (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define LOG_MSG(Format, ...)		  UE_LOG(CustomLog, Log, TEXT("%s %s"), *LINEINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOG_WARNING(Format, ...)	  UE_LOG(CustomLog, Warning, TEXT("%s %s"), *LINEINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOG_ERROR(Format, ...)		  UE_LOG(CustomLog, Error, TEXT("%s %s"), *LINEINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOG_FATAL(Format, ...)		  UE_LOG(CustomLog, Fatal, TEXT("%s %s"), *LINEINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOG_VERBOSE(Format, ...)	  UE_LOG(CustomLog, Verbose, TEXT("%s %s"), *LINEINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOG_VERYVERBOSE(Format, ...)  UE_LOG(CustomLog, VeryVerbose, TEXT("%s %s"), *LINEINFO, *FString::Printf(Format, ##__VA_ARGS__))

// BLog        : 그냥 텍스트 띄우고 싶을 떄
// BWarning    : 문제있음
// BError      : 치명적 오류

// Log	       : 로그파일 출력 O / 게임 내 콘솔 출력 X / 에디터의 출력로그 탭을 통해 계속 출력됨
// Warning     : 로그파일 출력 O / 게임 내 콘솔 출력 O / 노란색으로 표시됨
// Error       : 로그파일 출력 O / 게임 내 콘솔 출력 O / 빨간색으로 표시됨
// Display     : 로그파일 출력 O / 게임 내 콘솔 출력 O
// Fatal       : 로그파일 출력 O / 게임 내 콘솔 출력 O / 로그가 비활성회된 경우에도 작동이 중단, 중단 후 크래시 출력에서 라인과 텍스트 출력
// Verbose     : 로그파일 출력 O / 게임 내 콘솔 출력 X / 일반적으로 자세한 로깅 및 디버깅에 사용
// VeryVerbose : 로그파일 출력 O / 게임 내 콘솔 출력 X / 일반적으로 대량의 로그를 출력하는 상세한 로깅에 사용