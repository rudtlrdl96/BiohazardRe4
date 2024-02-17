// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BLeon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Generic/BFsm.h"
#include "Components/BoxComponent.h"
#include "../../Map/BJumpObstacleTrigger.h"
#include "../../Map/BCliffLineTrigger.h"
#include "Actor/Map/BMapBaseInteraction.h"
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

	if (nullptr == InteractionObject)
	{
		return;
	}

	if (false == AbleInteraction())
	{
		return;
	}

	EInteraction InteractionType = InteractionObject->GetInteractionType();

	switch (InteractionType)
	{
	case EInteraction::OpenDoor:
	{
		ABMapBaseInteraction* DoorInteraction = Cast<ABMapBaseInteraction>(InteractionActor);
		DoorInteraction->MapObjFastOpen(GetActorLocation());
	}
		return;
	case EInteraction::JumpObstacle:
	{
		ABJumpObstacleTrigger* TriggerActor = Cast<ABJumpObstacleTrigger>(InteractionActor);

		if (nullptr == TriggerActor)
		{
			LOG_FATAL(TEXT("Only classes that inherit ABJumpObstacleTrigger can have a JumpObstacle Interaction Type"));
			return;
		}

		FJumpData JumpData = TriggerActor->GetJumpMetaData(GetActorLocation());

		if (false == JumpData.bAbleJump)
		{
			return;
		}

		FVector ActorForward = GetActorForwardVector();
		ActorForward.Z = 0;
		ActorForward.Normalize();

		float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ActorForward, JumpData.MoveVector)));

		if (45 < Angle)
		{
			return;
		}

		JumpStart = JumpData.Start;
		JumpEnd = JumpData.End;
		JumpDir = JumpData.MoveVector;

		FsmComp->ChangeState(TO_KEY(ELeonState::ObstacleJump));
	}
		return;
	case EInteraction::FallCliff:
	{
		ABCliffLineTrigger* TriggerActor = Cast<ABCliffLineTrigger>(InteractionActor);

		if (nullptr == TriggerActor)
		{
			LOG_FATAL(TEXT("Only classes that inherit ABCliffLineTrigger can have a FallCliff Interaction Type"));
			return;
		}

		FJumpData JumpData = TriggerActor->GetJumpMetaData(GetActorLocation());

		if (false == JumpData.bAbleJump)
		{
			return;
		}

		FVector ActorForward = GetActorForwardVector();
		ActorForward.Z = 0;
		ActorForward.Normalize();

		float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ActorForward, JumpData.MoveVector)));

		if (45 < Angle)
		{
			return;
		}

		JumpStart = JumpData.Start;
		JumpEnd = JumpData.End;
		JumpDir = JumpData.MoveVector;

		if (300 < JumpStart.Z - JumpEnd.Z)
		{
			FallAnimation = ELeonFallAnimation::HighHeight;
		}
		else
		{
			FallAnimation = ELeonFallAnimation::LowHeight;
		}

		FsmComp->ChangeState(TO_KEY(ELeonState::Fall));
	}
		return;
	}
}

void ABLeon::JogExit()
{
	bIsJog = false;
	bIsMove = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
}