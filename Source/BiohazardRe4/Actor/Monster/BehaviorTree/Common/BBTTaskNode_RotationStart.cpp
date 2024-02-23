// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/Common/BBTTaskNode_RotationStart.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/Monster/Interface/BMonsterStateInterface.h"

EBTNodeResult::Type UBBTTaskNode_RotationStart::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APawn* MyPawn = Cast<APawn>(MyController->GetPawn());
	if (MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* MyCharacter = Cast<ACharacter>(MyPawn);
	if (MyCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IBMonsterStateInterface* StateInterface = Cast<IBMonsterStateInterface>(MyPawn);
	if (StateInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	MyCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

	return EBTNodeResult::Succeeded;
}
