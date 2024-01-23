// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/Component/BMonsterStatComponent.h"

// Sets default values for this component's properties
UBMonsterStatComponent::UBMonsterStatComponent()
{
	AttackRadius = 100.0f;
	MaxHp = 100;
	AttackSweepRadius = 30.0f;
	DetectRadius = 200.0f;
	PatrolRadius = 500.0f;
}

// Called when the game starts
void UBMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}


