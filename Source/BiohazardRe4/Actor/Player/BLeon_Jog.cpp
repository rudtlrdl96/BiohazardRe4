// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/BLeon.h"

void ABLeon::JogEnter()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : Jog Enter"));

	bIsJog = true;
	bIsMove = true;
}

void ABLeon::JogUpdate(float _DeltaTime)
{

}

void ABLeon::JogExit()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : Jog Exit"));

	bIsJog = false;
	bIsMove = false;
}