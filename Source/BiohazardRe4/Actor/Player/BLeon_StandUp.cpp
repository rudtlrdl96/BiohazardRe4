// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/BLeon.h"
#include "Generic/BFsm.h"

void ABLeon::StandUpEnter()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : StandUp Enter"));
}
 
void ABLeon::StandUpUpdate(float _DeltaTime)
{
	UE_LOG(LogTemp, Display, TEXT("FSM : StandUp Update"));

	FsmComp->ChangeState(TO_KEY(ELeonState::Walk));
}

void ABLeon::StandUpExit()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : StandUp Exit"));
}