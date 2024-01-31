// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BLeon.h"
#include "Generic/BFsm.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BiohazardRe4.h"

void ABLeon::AimEnter()
{
	bIsJog = false;
	bIsCrouch = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	AimUpdateTime = 0.0f;
	LeonAim = ELeonAim::Start;
}

void ABLeon::AimUpdate(float _DeltaTime)
{
	bIsCrouch = false;

	MoveDir = FMath::VInterpConstantTo(MoveDir, MoveInput, _DeltaTime, 6.0f);
	Aim(_DeltaTime);

	if (LeonWeaponSwap != ELeonWeaponSwap::None)
	{
		bIsAim = false;
		AimUpdateTime = 0.0f;
	}

	if (true == bIsAim)
	{
		AimUpdateTime = FMath::FInterpConstantTo(AimUpdateTime, 1.0f, _DeltaTime, 3.0f);

		if (1.0f <= AimUpdateTime)
		{
			LeonAim = ELeonAim::Loop;
		}
		else
		{
			LeonAim = ELeonAim::Start;
		}
	}
	else
	{		
		AimUpdateTime = FMath::FInterpConstantTo(AimUpdateTime, 0.0f, _DeltaTime, 5.0f);

		LeonAim = ELeonAim::End;

		if (0.0 < AimUpdateTime)
		{
			return;
		}

		if (MoveInput == FVector::ZeroVector)
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

void ABLeon::AimExit()
{
	LeonAim = ELeonAim::Start;

	GetCharacterMovement()->bOrientRotationToMovement = false;
}