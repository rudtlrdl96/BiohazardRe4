// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/Animation/AnimInstance/BBasicMonsterAnimInstance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Define/MonsterDefine.h"
#include "GameFramework/Character.h"
#include "BiohazardRe4.h"

void UBBasicMonsterAnimInstance::NativeUpdateAnimation(float _DeltaSeconds)
{
	Super::NativeUpdateAnimation(_DeltaSeconds);

	if (Owner == nullptr)
	{
		return;
	}

	AAIController* AIController = Cast<AAIController>(Owner->GetController());
	UObject* Target = AIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET);
	
	if (Target == nullptr)
	{
		return;
	}

	TraceTarget = Target;
}
