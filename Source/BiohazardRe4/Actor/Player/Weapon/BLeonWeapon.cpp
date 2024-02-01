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

void ABLeonWeapon::SetPlayer(ABLeon* _PlayerPtr)
{
	if (nullptr == _PlayerPtr)
	{
		LOG_FATAL(TEXT("nullptr PlayerPtr"));
	}

	Player = _PlayerPtr;
}

bool ABLeonWeapon::AbleReload() const
{
	return true;
}


void ABLeonWeapon::Reload()
{

}

bool ABLeonWeapon::AbleAttack() const
{
	return true;
}

void ABLeonWeapon::Attack()
{
	LOG_MSG(TEXT("Weapon Attack"));
}


uint32 ABLeonWeapon::GetAmmo() const
{
	return 1;
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