// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BLeon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BiohazardRe4.h"
#include "Generic/BFsm.h"
#include "Generic/BCollisionObserverCapsule.h"

void ABLeon::JumpObstacleEnter()
{
	bIsJog = false;
	bIsMove = false;
	bIsCrouch = false;
	bIsPlayGetItem = false;
	
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_None;
	JumpState = ELeonJumpState::MoveStart;

	MoveInteractionStartLerp = 0.0f;
	InteractionStartLocation = GetActorLocation();
	InteractionStartRotation = GetActorRotation();
	SpringArm->ProbeChannel = ECollisionChannel::ECC_GameTraceChannel14;
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetMesh()->SetCollisionProfileName("NoCollision");

}

void ABLeon::JumpObstacleUpdate(float _DeltaTime)
{
	if (JumpState == ELeonJumpState::MoveStart)
	{
		MoveInteractionStartLerp += _DeltaTime * 10;

		if (1.0f < MoveInteractionStartLerp)
		{
			MoveInteractionStartLerp = 1.0f;
			JumpState = ELeonJumpState::Jump;

			GetCharacterMovement()->Velocity = FVector::ZeroVector;
			GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
		}

		SetActorLocation(FMath::Lerp(InteractionStartLocation, JumpStart, MoveInteractionStartLerp));
		SetActorRotation(FMath::Lerp(InteractionStartRotation, JumpDir.Rotation(), MoveInteractionStartLerp));
	}
	else if (JumpState == ELeonJumpState::End)
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

void ABLeon::JumpObstacleExit()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SpringArm->ProbeChannel = ECollisionChannel::ECC_GameTraceChannel5;
	InteractionCoolTime = -0.2f;
}