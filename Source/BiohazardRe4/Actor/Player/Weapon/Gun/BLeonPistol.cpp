// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Gun/BLeonPistol.h"

void ABLeonPistol::BeginPlay()
{
	Super::BeginPlay();
	DefaultDamage = 1.f;
	RateOfFire = 1.f;
}
