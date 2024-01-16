// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/BLeon.h"

void ABLeon::CrouchEnter()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : Crouch Enter"));

	bIsCrouch = true;
	bIsMove = true;
}

void ABLeon::CrouchUpdate(float _DeltaTime)
{

}

void ABLeon::CrouchExit()
{
	UE_LOG(LogTemp, Display, TEXT("FSM : Crouch Exit"));

	bIsCrouch = false;
	bIsMove = false;
}