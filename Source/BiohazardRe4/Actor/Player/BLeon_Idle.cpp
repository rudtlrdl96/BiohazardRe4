// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/BLeon.h"
#include "Generic/BFsm.h"

void ABLeon::IdleEnter()
{
	bIsMove = false;
	bIsJog = false;
}
 
void ABLeon::IdleUpdate(float _DeltaTime)
{
	MoveDir = FMath::VInterpConstantTo(MoveDir, MoveInput, _DeltaTime, 6.0f);

	if (MoveInput == FVector::ZeroVector)
	{
		return;
	}

	if (true == bIsJogTrigger)
	{
		FsmComp->ChangeState(TO_KEY(ELeonState::Jog));
	}
	else
	{
		FsmComp->ChangeState(TO_KEY(ELeonState::Walk));
	}
}

void ABLeon::IdleExit()
{
}