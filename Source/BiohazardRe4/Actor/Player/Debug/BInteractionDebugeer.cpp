// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Debug/BInteractionDebugeer.h"

// Sets default values
ABInteractionDebugeer::ABInteractionDebugeer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
}

// Called when the game starts or when spawned
void ABInteractionDebugeer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABInteractionDebugeer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

