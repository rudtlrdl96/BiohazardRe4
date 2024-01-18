// Fill out your copyright notice in the Description page of Project Settings.


#include "BiohazardRe4.h"
#include "GameModeBase/BBiohazardRe4GameModeBase.h"

ABBiohazardRe4GameModeBase::ABBiohazardRe4GameModeBase()
{
	LOG_LOG(TEXT("Test Log"));
	LOG_WARNING(TEXT("Test Warning"));
	LOG_ERROR(TEXT("Test Error"));
	//LOG_FATAL(TEXT("Test fatal"));
	LOG_VERBOSE(TEXT("Test Verbose"));
	LOG_VERYVERBOSE(TEXT("Test Very Verbose"));
}