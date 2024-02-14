// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Grenade/BLeonFlashBang.h"
#include "DamageType/PlayerDamageType/BDMGPlayerFlashBang.h"

ABLeonFlashBang::ABLeonFlashBang()
{
	DamageType = UBDMGPlayerFlashBang::StaticClass();
	Damage = 0.f;
}