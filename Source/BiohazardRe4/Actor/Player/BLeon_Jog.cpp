// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/BLeon.h"
#include "Generic/BFsm.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BiohazardRe4.h"

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
	MoveDir = FMath::VInterpConstantTo(MoveDir, MoveInput, _DeltaTime, 6.0f);

	if (MoveInput == FVector::ZeroVector)
	{
		FsmComp->ChangeState(TO_KEY(ELeonState::Idle));
		return;
	}

	JogLookAt(_DeltaTime);
}

void ABLeon::JogExit()
{
	bIsJog = false;
	bIsMove = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
}