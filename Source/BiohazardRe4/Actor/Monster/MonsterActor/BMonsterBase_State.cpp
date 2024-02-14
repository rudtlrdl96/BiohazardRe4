// Fill out your copyright notice in the Description page of Project Settings.


#include "BMonsterBase.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BiohazardRe4.h"
#include "Actor/Monster/Define/MonsterDefine.h"

void ABMonsterBase::SetMonsterAttackEndDelegate(FMonsterAttackEnd& _InAttackEnd)
{
	OnAttackEnd = _InAttackEnd;
}

const FMonsterAttackEnd& ABMonsterBase::GetMonsterAttackEndDelegate()
{
	return OnAttackEnd;
}

bool ABMonsterBase::isAbleParring()
{
	return bIsAbleParry;
}

void ABMonsterBase::ParryTimeOn()
{
	bIsAbleParry = true;
}

void ABMonsterBase::ParryTimeOff()
{
	bIsAbleParry = false;
}

void ABMonsterBase::SetCurrentState(EMonsterState _InState)
{
	CurState = _InState;
}

bool ABMonsterBase::isDamaged()
{
	return bIsDamaged;
}

EMonsterState ABMonsterBase::GetCurrentState()
{
	return CurState;
}

void ABMonsterBase::DamagedEnd()
{
	bIsDamaged = false;
}

void ABMonsterBase::GroggyEnd()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		LOG_WARNING(TEXT("AIController is nullptr"));
		return;
	}

	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISDAMAGED, false);
	SetCurrentState(EMonsterState::Walk);
	bIsDamaged = false;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		LOG_WARNING(TEXT("AnimInstance is Nullptr"));
		return;
	}

	AnimInstance->StopAllMontages(1.0f);
	AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
}