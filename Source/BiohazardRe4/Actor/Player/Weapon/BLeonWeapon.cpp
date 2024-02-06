// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/BLeonWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BiohazardRe4.h"

// Sets default values
ABLeonWeapon::ABLeonWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
	ProjectileComp->bSimulationEnabled = false;
	ProjectileComp->bShouldBounce = true;	
	ProjectileComp->Bounciness = 0.2;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetCollisionProfileName("Weapon");
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

void ABLeonWeapon::ThrowWeapon(const FVector& _Velocity)
{
	if (nullptr == ProjectileComp)
	{
		LOG_FATAL(TEXT("nullptr ProjectileComp"));
	}

	ProjectileComp->bSimulationEnabled = true;
	ProjectileComp->Velocity = _Velocity;
}


void ABLeonWeapon::ActiveSimulatePhysics(bool _bIsActive)
{
	WeaponMesh->SetSimulatePhysics(_bIsActive);
}