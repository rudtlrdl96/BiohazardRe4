// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BMapBaseInteraction.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "BiohazardRe4.h"

// Sets default values
ABMapBaseInteraction::ABMapBaseInteraction()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsOpen = false;
	bInter = false;
	bIsFastOpen = false;
	bIsClose = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent->SetMobility(EComponentMobility::Type::Movable);

	ATrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger A"));
	ATrigger->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ATrigger->SetRelativeLocation(FVector(FVector(-65, 0, 0)));
	ATrigger->SetBoxExtent(FVector(10, 100, 10));
	ATrigger->SetCollisionProfileName("Interaction");
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
	if (true == bIsOpen)
	{
		//LOG_MSG(TEXT("true"));
	}
	else
	{
		//LOG_MSG(TEXT("false"));
	}

	return false == bIsOpen;
}

EInteraction ABMapBaseInteraction::GetInteractionType() const
{
	return EInteraction::OpenDoor;
}

void ABMapBaseInteraction::MapObjOpen(const FVector& _Location)
{
	bIsOpen = true;
}

void ABMapBaseInteraction::MapObjClose(const FVector& _Location)
{
	bIsOpen = false;
}

void ABMapBaseInteraction::MapObjFastOpen(const FVector& _Location)
{
	bIsOpen = true;
}

