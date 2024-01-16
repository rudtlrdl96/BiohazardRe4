// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/BLeon.h"

void ABLeon::WalkEnter()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : Walk Enter"));

	bIsMove = true;
}

void ABLeon::WalkUpdate(float _DeltaTime)
{
}

void ABLeon::WalkExit()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : Walk Exit"));

	bIsMove = false;
}