// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BLeon.h"
#include "Generic/BFsm.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void ABLeon::WalkEnter()
{
	bIsJog = false;
	bIsMove = true;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ABLeon::WalkUpdate(float _DeltaTime)
{
	if (true == AbleAim() && true == bIsAim)
	{
		FsmComp->ChangeState(TO_KEY(ELeonState::Aim));
		return;
	}

	MoveDir = FMath::VInterpConstantTo(MoveDir, MoveInput, _DeltaTime, 6.0f);

	if (MoveInput == FVector::ZeroVector)
	{
		FsmComp->ChangeState(TO_KEY(ELeonState::Idle));
		return;
	}

	if (true == bIsJogTrigger)
	{
		FsmComp->ChangeState(TO_KEY(ELeonState::Jog));
		return;
	}
}

void ABLeon::WalkExit()
{
	bIsMove = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}