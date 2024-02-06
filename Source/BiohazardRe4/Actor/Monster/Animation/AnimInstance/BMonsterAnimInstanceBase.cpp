// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/Animation/AnimInstance/BMonsterAnimInstanceBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Interface/BMonsterStateInterface.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Define/MonsterDefine.h"
#include "BiohazardRe4.h"

UBMonsterAnimInstanceBase::UBMonsterAnimInstanceBase()
{
	//AnimType = EMonsterAnimType::BareHands;
	GroundSpeedThreshold = 0.0f;
}

void UBMonsterAnimInstanceBase::SetTarget(UObject* _Target)
{
	TraceTarget = _Target;
}

void UBMonsterAnimInstanceBase::SetAnimationType(EMonsterAnimType _AnimType)
{
	AnimType = _AnimType;

	if (AnimType == EMonsterAnimType::BareHands)
	{
		//LOG_MSG(TEXT("Monster Bare Hands Update"));
	}
	else if (AnimType == EMonsterAnimType::OneHand)
	{
		//LOG_MSG(TEXT("Monster One Hand Update"));
	}
}

void UBMonsterAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	//
	Owner = Cast<ACharacter>(GetOwningActor());
	
	if (Owner != nullptr)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UBMonsterAnimInstanceBase::NativeUpdateAnimation(float _DeltaSeconds)
{
	Super::NativeUpdateAnimation(_DeltaSeconds);

	if (Owner == nullptr)
	{
		return;
	}

	if (Movement != nullptr)
	{
		GroundSpeed = Movement->Velocity.Size2D();
		GroundRotation = Owner->GetActorRotation();
	}

	IBMonsterStateInterface* StateInterface = Cast<IBMonsterStateInterface>(GetOwningActor());
	
	if (StateInterface != nullptr)
	{
		CurState = StaticCast<uint8>(StateInterface->GetCurrentState());
	}

	if (TraceTarget != nullptr)
	{
		ACharacter* Character = Cast<ACharacter>(TraceTarget);
		if (Character == nullptr)
		{
			LOG_WARNING(TEXT("Character is nullptr"));
			return;
		}

		DistanceToPlayer = Owner->GetDistanceTo(Character);
	}
}
