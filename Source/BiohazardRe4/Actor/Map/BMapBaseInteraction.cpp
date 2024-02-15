// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BMapBaseInteraction.h"

// Sets default values
ABMapBaseInteraction::ABMapBaseInteraction()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsOpen = false;
}

// Called when the game starts or when spawned
void ABMapBaseInteraction::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABMapBaseInteraction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ABMapBaseInteraction::AbleInteraction() const
{
	return true;
}

EInteraction ABMapBaseInteraction::GetInteractionType() const
{
	return EInteraction::OpenDoor;
}

void ABMapBaseInteraction::MapObjOpen()
{
	bIsOpen = true;
}

void ABMapBaseInteraction::MapObjClose()
{
	bIsOpen = false;
}

