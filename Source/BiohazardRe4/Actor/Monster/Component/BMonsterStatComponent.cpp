// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/Component/BMonsterStatComponent.h"

// Sets default values for this component's properties
UBMonsterStatComponent::UBMonsterStatComponent()
{
	AttackRange = 100.0f;
	MaxHp = 10000000;
}


// Called when the game starts
void UBMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}


