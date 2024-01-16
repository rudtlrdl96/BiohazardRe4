// Fill out your copyright notice in the Description page of Project Settings.


#include "Title/BTItleBackgroundObj.h"

// Sets default values
ABTItleBackgroundObj::ABTItleBackgroundObj()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABTItleBackgroundObj::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABTItleBackgroundObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CurPos = FMath::VInterpConstantTo(GetActorLocation(), EndLocation, DeltaTime, Velocity);
	SetActorLocation(CurPos);
	if (FVector::Dist(EndLocation, CurPos) == 0)
	{
		SetActorLocation(StartLocation);
	}

	FVector CurScale = FVector::One() * (1 - GetRatio());
	SetActorScale3D(CurScale);

}

