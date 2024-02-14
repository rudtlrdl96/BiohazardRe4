// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Grenade/BLeonGrenade.h"
#include "Generic/BCollisionObserverSphere.h"
#include "DamageType/PlayerDamageType/BDMGPlayerGranade.h"
#include "Kismet/GameplayStatics.h"


ABLeonGrenade::ABLeonGrenade()
{
	DamageType = UBDMGPlayerGranade::StaticClass();
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
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 30, FColor::Green, false, 2.f);

	UGameplayStatics::ApplyRadialDamage
	(
		GetWorld(),
		Damage,
		GetActorLocation(),
		Radius,
		DamageType,
		IgnoreDamageActor,
		this,
		Player->GetController(),
		false,
		ECollisionChannel::ECC_GameTraceChannel17
	);

	Destroy();
}

void ABLeonGrenade::Attack()
{
	IsThrowing = true;
}
