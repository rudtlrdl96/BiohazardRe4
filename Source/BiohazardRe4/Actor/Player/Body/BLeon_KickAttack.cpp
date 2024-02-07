// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BLeon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BiohazardRe4.h"
#include "Generic/BFsm.h"
#include "Generic/BCollisionObserverCapsule.h"

void ABLeon::KickAttackEnter()
{
	bIsJog = false;
	bIsMove = false;
	bIsCrouch = false;
	bIsKickEnd = false;
	bIsKickAttackActive = false;
	bIsPlayGetItem = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	KickOverlapObserver->SetVisibilityCollision(false);
}

void ABLeon::KickAttackUpdate(float _DeltaTime)
{
	FVector KickVector = KickLocation - GetActorLocation();
	KickVector.Z = 0;
	KickVector.Normalize();
	FRotator Rotator = KickVector.Rotation();

	SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), Rotator, _DeltaTime, 360.0f));

	if (true == bIsKickEnd)
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

void ABLeon::KickAttackExit()
{
	bIsKickEnd = false;
	bIsKickAttackActive = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	KickOverlapObserver->SetVisibilityCollision(false);
}