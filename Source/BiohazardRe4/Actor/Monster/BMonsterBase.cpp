// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BMonsterBase.h"
#include "Component/BMonsterStatComponent.h"

// Sets default values
ABMonsterBase::ABMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	Stat = CreateDefaultSubobject<UBMonsterStatComponent>(TEXT("Stat"));
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

void ABMonsterBase::SetMonsterAttackEndDelegate(FMonsterAttackEnd& _InAttackEnd)
{
	OnAttackEnd = _InAttackEnd;
}

const FMonsterAttackEnd& ABMonsterBase::GetMonsterAttackEndDelegate()
{
	return OnAttackEnd;
}

float ABMonsterBase::GetAttackRange()
{
	return Stat->GetAttackRange();
}

