// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/BLeon.h"

void ABLeon::StandUpEnter()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : StandUp Enter"));

	bIsMove = false;
	bIsJog = false;
	bIsCrouch = false;
}
 
void ABLeon::StandUpUpdate(float _DeltaTime)
{

}

void ABLeon::StandUpExit()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : StandUp Exit"));
}