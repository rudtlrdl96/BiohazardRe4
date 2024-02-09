// Fill out your copyright notice in the Description page of Project Settings.


#include "BMonsterStatComponent.h"
#include "BiohazardRe4.h"
#include "Actor/Monster/Interface/BMonsterStateInterface.h"
// Sets default values for this component's properties
UBMonsterStatComponent::UBMonsterStatComponent()
{
}

void UBMonsterStatComponent::StatInit(const FStatStruct& _StatData)
{
	MaxHp = _StatData.MaxHp;
	CurrentHp = _StatData.CurrentHp;
	
	AttackRadius = _StatData.AttackRadius;
	AttackSweepRadius = _StatData.AttackSweepRadius;
	
	DetectRadius = _StatData.DetectRadius;
	PatrolRadius = _StatData.PatrolRadius;

	WalkSPeed = _StatData.WalkSpeed;
	RunSpeed = _StatData.RunSpeed;
	WalkDistanceThreshold = _StatData.WalkDistanceThreshold;

	BaseAttackPower = _StatData.BaseAttackPower;
}

void UBMonsterStatComponent::DecreaseHp(float _Hp)
{
	CurrentHp -= _Hp;
}

// Called when the game starts
void UBMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}


