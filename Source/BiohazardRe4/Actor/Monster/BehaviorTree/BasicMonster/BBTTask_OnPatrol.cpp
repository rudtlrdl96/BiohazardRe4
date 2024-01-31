// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BasicMonster/BBTTask_OnPatrol.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBBTTask_OnPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* MyCharacter = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if (nullptr == MyCharacter)
	{
		return EBTNodeResult::Failed;
	}

	MyCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	return EBTNodeResult::Type();
}
