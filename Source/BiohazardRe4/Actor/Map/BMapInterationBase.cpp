// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BMapInterationBase.h"

// Sets default values
ABMapInterationBase::ABMapInterationBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABMapInterationBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABMapInterationBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

