// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BMapDoorInteraction.h"
#include "Components/BoxComponent.h"
#include "BiohazardRe4.h"
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
	FVector DoorFront = ATrigger->GetForwardVector();
	DoorFront.Z = 0;
	DoorFront.Normalize();
	FVector Direction = _Location - ATrigger->GetComponentLocation();
	Direction.Z = 0;
	Direction.Normalize();

	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(DoorFront, Direction)));
	LOG_MSG(TEXT("%f"),Angle);
	if (90 < Angle)
	{
		return EDoorState::Front;
	}
	else
	{
		return EDoorState::Back;
	}
}
