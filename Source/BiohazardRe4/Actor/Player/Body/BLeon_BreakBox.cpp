// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BLeon.h"
#include "Generic/BFsm.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BiohazardRe4.h"

void ABLeon::BreakBoxEnter()
{
	bIsJog = false;
	bIsMove = false;
	bIsCrouch = false;
	bIsPlayGetItem = false;

	KnifeAttackState = ELeonKnifeAttackState::EnterAttack;

	bAbleComboInput = false;
	bIsComboEnd = false;
}

void ABLeon::BreakBoxUpdate(float _DeltaTime)
{
	if (true == bIsComboEnd)
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

void ABLeon::BreakBoxExit()
{
	bAbleComboInput = false;
	bIsComboEnd = false;
}