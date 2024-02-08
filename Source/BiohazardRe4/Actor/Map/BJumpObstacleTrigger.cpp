// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BJumpObstacleTrigger.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ABJumpObstacleTrigger::ABJumpObstacleTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	LocationATrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger A"));
	LocationATrigger->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	LocationATrigger->SetRelativeLocation(FVector(FVector(-65, 0, 0)));
	LocationATrigger->SetBoxExtent(FVector(1, 1, 10));
	LocationATrigger->SetCollisionProfileName("Interaction");

	LocationBTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger B"));
	LocationBTrigger->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	LocationBTrigger->SetRelativeLocation(FVector(FVector(65, 0, 0)));
	LocationBTrigger->SetBoxExtent(FVector(1, 1, 10));
	LocationBTrigger->SetCollisionProfileName("Interaction");
}

// Called when the game starts or when spawned
void ABJumpObstacleTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABJumpObstacleTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


FJumpData ABJumpObstacleTrigger::GetJumpMetaData(const FVector& _Location) const
{
	FJumpData Result;

	Result.bAbleJump = AbleInteraction();

	float DistanceA = FVector::Distance(_Location, LocationATrigger->GetComponentLocation());
	float DistanceB = FVector::Distance(_Location, LocationBTrigger->GetComponentLocation());

	if (DistanceA > DistanceB)
	{
		Result.Start = LocationBTrigger->GetComponentLocation();
		Result.End = LocationATrigger->GetComponentLocation();
	}
	else
	{
		Result.Start = LocationATrigger->GetComponentLocation();
		Result.End = LocationBTrigger->GetComponentLocation();
	}

	Result.MoveVector = Result.End - Result.Start;
	Result.MoveVector.Normalize();

	return Result;
}