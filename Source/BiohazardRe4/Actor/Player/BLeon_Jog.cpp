// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/BLeon.h"
#include "Generic/BFsm.h"

void ABLeon::JogEnter()
{
	bIsJog = true;
	bIsMove = true;
	bIsCrouch = false;
}

void ABLeon::JogUpdate(float _DeltaTime)
{
	MoveDir = FMath::VInterpConstantTo(MoveDir, MoveInput, _DeltaTime, 6.0f);

	if (MoveInput == FVector::ZeroVector)
	{
		FsmComp->ChangeState(TO_KEY(ELeonState::Idle));
		return;
	}
}

void ABLeon::JogExit()
{
	bIsJog = false;
	bIsMove = false;
}