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

// BLog        : �׳� �ؽ�Ʈ ���� ���� ��
// BWarning    : ��������
// BError      : ġ���� ����

// Log	       : �α����� ��� O / ���� �� �ܼ� ��� X / �������� ��·α� ���� ���� ��� ��µ�
// Warning     : �α����� ��� O / ���� �� �ܼ� ��� O / ��������� ǥ�õ�
// Error       : �α����� ��� O / ���� �� �ܼ� ��� O / ���������� ǥ�õ�
// Display     : �α����� ��� O / ���� �� �ܼ� ��� O
// Fatal       : �α����� ��� O / ���� �� �ܼ� ��� O / �αװ� ��Ȱ��ȸ�� ��쿡�� �۵��� �ߴ�, �ߴ� �� ũ���� ��¿��� ���ΰ� �ؽ�Ʈ ���
// Verbose     : �α����� ��� O / ���� �� �ܼ� ��� X / �Ϲ������� �ڼ��� �α� �� ����뿡 ���
// VeryVerbose : �α����� ��� O / ���� �� �ܼ� ��� X / �Ϲ������� �뷮�� �α׸� ����ϴ� ���� �α뿡 ���