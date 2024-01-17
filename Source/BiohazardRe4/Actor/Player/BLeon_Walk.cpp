// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/BLeon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void ABLeon::WalkEnter()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : Walk Enter"));

	bIsMove = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ABLeon::WalkUpdate(float _DeltaTime)
{
	//FRotator PlayerRot = GetActorRotation();
	//FRotator CameraRot = GetControlRotation();
	//
	//SetActorRotation(FMath::RInterpConstantTo(PlayerRot, CameraRot, _DeltaTime, 6.0f));
}

void ABLeon::WalkExit()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : Walk Exit"));

	bIsMove = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}