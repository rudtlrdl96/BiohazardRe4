// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/BLeon.h"

void ABLeon::IdleEnter()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : StandUp Enter"));

	bIsMove = false;
	bIsJog = false;
}
 
void ABLeon::IdleUpdate(float _DeltaTime)
{

}

void ABLeon::IdleExit()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : StandUp Exit"));
}