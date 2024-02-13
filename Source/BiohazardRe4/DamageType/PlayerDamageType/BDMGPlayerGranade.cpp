// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageType/PlayerDamageType/BDMGPlayerGranade.h"

UBDMGPlayerGranade::UBDMGPlayerGranade()
{
	WeaponItemCode = EItemCode::Grenade;
	DamageType = EPlayerDamageType::Grenade;
}