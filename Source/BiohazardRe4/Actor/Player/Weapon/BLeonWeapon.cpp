// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/BLeonWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "BiohazardRe4.h"

// Sets default values
ABLeonWeapon::ABLeonWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;	
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

void ABLeonWeapon::Attack()
{
	LOG_MSG(TEXT("Weapon Attack"));
}

bool ABLeonWeapon::AbleAttack()
{
	return true;
}

FTransform ABLeonWeapon::GetLeftHandSocketTransform() const
{
	const USkeletalMeshSocket* SocketPtr = WeaponMesh->GetSocketByName(TEXT("LeftHandSocket"));

	if (nullptr == SocketPtr)
	{
		return RootComponent->GetComponentTransform();
	}
	
	return SocketPtr->GetSocketTransform(WeaponMesh);
}