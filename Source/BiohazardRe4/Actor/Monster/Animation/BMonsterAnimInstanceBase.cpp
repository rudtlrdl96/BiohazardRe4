// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/Animation/BMonsterAnimInstanceBase.h"
#include "GameFramework/Character.h"

UBMonsterAnimInstanceBase::UBMonsterAnimInstanceBase()
{

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

	IBMonsterStateInterface* StateInterface = Cast<IBMonsterStateInterface>(GetOwningActor());
	
	if (StateInterface != nullptr)
	{
		CurState = StaticCast<uint8>(StateInterface->GetCurrentState());
	}
}
