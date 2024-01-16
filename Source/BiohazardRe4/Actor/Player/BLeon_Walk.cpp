// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/BLeon.h"
#include "Generic/BFsm.h"

void ABLeon::WalkEnter()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : Walk Enter"));
}

void ABLeon::WalkUpdate(float _DeltaTime)
{
	UE_LOG(LogTemp, Display, TEXT("FSM : Walk Update"));
}

void ABLeon::WalkExit()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : Walk Exit"));
}