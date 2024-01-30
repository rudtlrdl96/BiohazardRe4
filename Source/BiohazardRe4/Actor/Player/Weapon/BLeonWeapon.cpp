// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/BLeonWeapon.h"

// Sets default values
ABLeonWeapon::ABLeonWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;

	LeftHandSlot = CreateDefaultSubobject<USceneComponent>(TEXT("Hand Slot"));
}

// Called when the game starts or when spawned
void ABLeonWeapon::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ABLeonWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABLeonWeapon::Trigger()
{
}

void ABLeonWeapon::Reload()
{
}