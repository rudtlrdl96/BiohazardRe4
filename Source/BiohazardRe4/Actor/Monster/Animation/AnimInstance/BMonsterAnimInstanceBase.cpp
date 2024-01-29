// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/Animation/AnimInstance/BMonsterAnimInstanceBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UBMonsterAnimInstanceBase::UBMonsterAnimInstanceBase()
{
	GroundSpeedThreshold = 0.0f;
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
}
