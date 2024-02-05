// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Gun/BLeonShotgun.h"

void ABLeonShotgun::BeginPlay()
{
	Super::BeginPlay();
	DefaultDamage = 5.6f;
	RateOfFire = 0.45f;
	Range = 2500.f;
	MaxAmmo = 5;
}
