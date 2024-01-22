// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/Component/BMonsterStatComponent.h"

// Sets default values for this component's properties
UBMonsterStatComponent::UBMonsterStatComponent()
{
	AttackRange = 125.0f;
	MaxHp = 100;
	WeaponAttackRadius = 15.0f;
	DetectRange = 200.0f;
	PatrolRadius = 500.0f;
}

// Called when the game starts
void UBMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}


