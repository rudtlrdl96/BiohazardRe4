// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Gun/BLeonRifle.h"

void ABLeonRifle::BeginPlay()
{
	Super::BeginPlay();
	DamageUnit = 100.f;
	DefaultDamage = 2.5f;
	RateOfFire = 0.42f;
	Range = 10000.f;
}
