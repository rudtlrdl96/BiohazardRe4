// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BMonsterBase.h"

// Sets default values
ABMonsterBase::ABMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
}

void ABMonsterBase::BeginPlay()
{
	Super::BeginPlay();
}

MonsterState ABMonsterBase::GetCurrentState()
{
	return CurState;
}

void ABMonsterBase::SetCurrentState(MonsterState _InState)
{
	CurState = _InState;
}

bool ABMonsterBase::IsAttacking()
{
	return bIsAttacking;
}

void ABMonsterBase::SetIsAttack(bool _IsAttacking)
{
	bIsAttacking = _IsAttacking;
}

