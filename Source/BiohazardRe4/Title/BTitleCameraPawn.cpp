// Fill out your copyright notice in the Description page of Project Settings.


#include "Title/BTitleCameraPawn.h"

// Sets default values
ABTitleCameraPawn::ABTitleCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABTitleCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABTitleCameraPawn::Tick(float DeltaTime)
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

// Called to bind functionality to input
void ABTitleCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

