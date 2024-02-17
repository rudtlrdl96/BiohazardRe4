// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BMapDoorInteraction.h"

ABMapDoorInteraction::ABMapDoorInteraction()
{
	bInValue = false;
	bFront = false;
	bBack = false;
}

void ABMapDoorInteraction::MapObjOpen()
{
	DoorSpeed = EDoorSpeed::Slow;
	Super::MapObjOpen();
}

void ABMapDoorInteraction::MapObjFastOpen()
{
	DoorSpeed = EDoorSpeed::Fast;
	Super::MapObjFastOpen();
}
