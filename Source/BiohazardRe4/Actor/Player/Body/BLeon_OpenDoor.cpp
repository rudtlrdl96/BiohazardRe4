// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BLeon.h"
#include "Generic/BFsm.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BiohazardRe4.h"

void ABLeon::OpenDoorEnter()
{
	bIsJog = false;
	bIsMove = false;
	bIsCrouch = false;
	bIsPlayGetItem = false;

	bIsOpenEnd = false;
}

void ABLeon::OpenDoorUpdate(float _DeltaTime)
{
	if (true == bIsOpenEnd)
	{
		if (true == AbleAim() && true == bIsAim)
		{
			FsmComp->ChangeState(TO_KEY(ELeonState::Aim));
			return;
		}
		else if (MoveInput == FVector::ZeroVector)
		{
			FsmComp->ChangeState(TO_KEY(ELeonState::Idle));
		}
		else if (true == bIsJogTrigger)
		{
			FsmComp->ChangeState(TO_KEY(ELeonState::Jog));
		}
		else
		{
			FsmComp->ChangeState(TO_KEY(ELeonState::Walk));
		}
	}
}

void ABLeon::OpenDoorExit()
{
	InteractionCoolTime = -0.2f;
}