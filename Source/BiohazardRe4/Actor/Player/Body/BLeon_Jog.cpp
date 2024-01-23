// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BLeon.h"
#include "Generic/BFsm.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void ABLeon::JogEnter()
{
	bIsJog = true;
	bIsMove = true;
	bIsCrouch = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	JogTurnAngle = 0.0;
}

void ABLeon::JogUpdate(float _DeltaTime)
{	
	if (true == AbleAim() && true == bIsAim)
	{
		FsmComp->ChangeState(TO_KEY(ELeonState::Aim));
		return;
	}

	MoveDir = FMath::VInterpConstantTo(MoveDir, MoveInput, _DeltaTime, 6.0f);
	JogLookAt(_DeltaTime);

	if (MoveInput == FVector::ZeroVector)
	{
		FsmComp->ChangeState(TO_KEY(ELeonState::Idle));
	}
}

void ABLeon::JogExit()
{
	bIsJog = false;
	bIsMove = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
}