// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BMapDoorInteraction.h"

ABMapDoorInteraction::ABMapDoorInteraction()
{
	bInValue = false;
}

void ABMapDoorInteraction::MapObjOpen(const FVector& _Location)
{
	DoorState = GetDoorDirection(_Location);
	DoorSpeed = EDoorSpeed::Slow;
	Super::MapObjOpen(_Location);
}

void ABMapDoorInteraction::MapObjFastOpen(const FVector& _Location)
{
	DoorState = GetDoorDirection(_Location);
	DoorSpeed = EDoorSpeed::Fast;
	Super::MapObjFastOpen(_Location);
}

EDoorState ABMapDoorInteraction::GetDoorDirection(const FVector& _Location)
{
	FVector DoorFront = GetActorForwardVector();
	DoorFront.Z = 0;

	FVector Direction = _Location - GetActorLocation();
	Direction.Z = 0;
	Direction.Normalize();

	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(DoorFront, Direction)));

	if (90 < Angle)
	{
		return EDoorState::Back;
	}
	else
	{
		return EDoorState::Front;
	}
}
