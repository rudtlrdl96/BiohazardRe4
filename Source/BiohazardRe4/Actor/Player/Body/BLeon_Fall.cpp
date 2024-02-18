// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BLeon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BiohazardRe4.h"
#include "Generic/BFsm.h"
#include "Generic/BCollisionObserverCapsule.h"

void ABLeon::FallEnter()
{
	bIsJog = false;
	bIsMove = false;
	bIsCrouch = false;
	bIsPlayGetItem = false;
	bIsFallEndCheck = false;
	bIsFallLandingEnd = false;
	
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;

	FallState = ELeonFallState::FallStart;

	MoveInteractionStartLerp = 0.0f;
	InteractionStartLocation = GetActorLocation();
	InteractionStartRotation = GetActorRotation();
	SpringArm->ProbeChannel = ECollisionChannel::ECC_GameTraceChannel14;
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetMesh()->SetCollisionProfileName("NoCollision");
}

void ABLeon::FallUpdate(float _DeltaTime)
{
	if (true == bIsFallEndCheck)
	{
		FHitResult HitResult;

		if (true == GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() + FVector(0, 0, -120), ECollisionChannel::ECC_GameTraceChannel2))
		{
			GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
			//GetMesh()->GetAnimInstance()->RootMotionMode = ERootMotionMode::Type::RootMotionFromEverything;
			FallState = ELeonFallState::FallEnd;
		}
	}

	if (FallState == ELeonFallState::FallStart)
	{
		MoveInteractionStartLerp += _DeltaTime * 20;

		if (1.0f < MoveInteractionStartLerp)
		{
			MoveInteractionStartLerp = 1.0f;
			FallState = ELeonFallState::FallLoop;

			GetCharacterMovement()->Velocity = FVector::ZeroVector;
			GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Falling;
		}

		//SetActorLocation(FMath::Lerp(InteractionStartLocation, JumpStart, MoveInteractionStartLerp));
		SetActorRotation(FMath::Lerp(InteractionStartRotation, JumpDir.Rotation(), MoveInteractionStartLerp));
	}
	else if (true == bIsFallLandingEnd)
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

void ABLeon::FallExit()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	SpringArm->ProbeChannel = ECollisionChannel::ECC_GameTraceChannel5;
	InteractionCoolTime = -0.2f;
}