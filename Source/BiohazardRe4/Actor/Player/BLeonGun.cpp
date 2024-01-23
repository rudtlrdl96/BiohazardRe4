// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/BLeonGun.h"

// Sets default values
ABLeonGun::ABLeonGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABLeonGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABLeonGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABLeonGun::Trigger()
{
}

void ABLeonGun::Shoot()
{
}

void ABLeonGun::Reload()
{
}

