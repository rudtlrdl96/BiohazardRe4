// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Grenade/BLeonGrenade.h"
#include "Generic/BCollisionObserverSphere.h"
#include "DamageType/PlayerDamageType/BDMGPlayerRifle.h"
#include "Kismet/GameplayStatics.h"


ABLeonGrenade::ABLeonGrenade()
{
	DamageType = UBDMGPlayerRifle::StaticClass();
}

void ABLeonGrenade::BeginPlay()
{
	Super::BeginPlay();
}

void ABLeonGrenade::Tick(float DeltaTime)
{
	if (IsThrowing)
	{
		Time += DeltaTime;

		if (Time >= LifeTime)
		{
			Explosion();
		}
	}
}

void ABLeonGrenade::Explosion()
{
	LOG_MSG(TEXT("Explosion"))
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 30, FColor::Green);

	UGameplayStatics::ApplyRadialDamage
	(
		GetWorld(),
		100,
		GetActorLocation(),
		Radius,
		DamageType,
		IgnoreDamageActor,
		this,
		Player->GetController(),
		false,
		ECollisionChannel::ECC_EngineTraceChannel4
	);

	Destroy();
}

void ABLeonGrenade::Attack()
{
	IsThrowing = true;
}
