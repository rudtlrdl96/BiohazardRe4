// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Grenade/BLeonGrenade.h"
#include "Generic/BCollisionObserverSphere.h"
#include "DamageType/PlayerDamageType/BDMGPlayerGranade.h"
#include "DamageType/PlayerDamageType/BDMGPlayerFlashBang.h"
#include "Kismet/GameplayStatics.h"
#include "Item/InventoryActor.h"


ABLeonGrenade::ABLeonGrenade()
{
	DamageType = UBDMGPlayerGranade::StaticClass();
	WeaponDamage = 1000.f;
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
		WeaponDamage,
		GetActorLocation(),
		Radius,
		DamageType,
		IgnoreDamageActor,
		this,
		Player->GetController(),
		false,
		ECollisionChannel::ECC_GameTraceChannel17
	);
	ExplosionEffect();
	PlayExplosionSound();
	Destroy();
}

void ABLeonGrenade::Attack()
{
	IsThrowing = true;
	InventoryInst->RemoveItem(WeaponType);
}

void ABLeonGrenade::SetFlashbang()
{
	WeaponType = EItemCode::Flashbang;
	DamageType = UBDMGPlayerFlashBang::StaticClass();
	WeaponDamage = 0.f;
}

void ABLeonGrenade::ExplosionEffect_Implementation()
{
	LOG_MSG(TEXT("ExplosionEffect_Implementation"))
}

void ABLeonGrenade::PlayExplosionSound()
{
	if (ExplosionSound == nullptr)
	{
		LOG_WARNING(TEXT("ExplosionSound == nullptr"))
			return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
}
