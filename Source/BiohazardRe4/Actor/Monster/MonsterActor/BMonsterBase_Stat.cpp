// Fill out your copyright notice in the Description page of Project Settings.


#include "BMonsterBase.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

#include "Actor/Monster/Component/BMonsterStatComponent.h"
#include "Actor/Monster/DataAsset/BMonsterStatData.h"

void ABMonsterBase::StatInit(const UBMonsterStatData* _DataAsset)
{
	FStatStruct StatStruct;

	StatStruct.MaxHp = _DataAsset->MaxHp;
	StatStruct.CurrentHp = _DataAsset->CurrentHp;

	StatStruct.DetectRadius = _DataAsset->DetectRadius;
	StatStruct.PatrolRadius = _DataAsset->PatrolRadius;

	StatStruct.WalkSpeed = _DataAsset->WalkSpeed;
	StatStruct.RunSpeed = _DataAsset->RunSpeed;
	StatStruct.WalkDistanceThreshold = _DataAsset->WalkDistanceThreshold;

	StatStruct.BaseAttackPower = _DataAsset->BaseAttackPower;
	StatStruct.GroggyThreshold = _DataAsset->GroggyThreshold;

	StatStruct.AttackRange = _DataAsset->AttackRange;

	Stat->StatInit(StatStruct);
}

float ABMonsterBase::GetDetectRadius() const
{
	return Stat->GetDetectRadius();
}

float ABMonsterBase::GetPatrolRadius() const
{
	return Stat->GetPatrolRadius();
}

float ABMonsterBase::GetGroggyThreshold() const
{
	return Stat->GetGroggyThreshold();
}

float ABMonsterBase::GetRunSpeed() const
{
	return Stat->GetRunSpeed();
}

float ABMonsterBase::GetWalkSpeed() const
{
	return Stat->GetWalkSpeed();
}

float ABMonsterBase::GetAttackRange() const
{
	return Stat->GetAttackRange();
}

float ABMonsterBase::GetWalkDistanceThreshold() const
{
	return Stat->GetWalkDistanceThreshold();
}
