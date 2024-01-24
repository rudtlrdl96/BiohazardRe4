// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/Common/BBTTask_SetTurn.h"
#include "AIController.h"
#include "../../Interface/BMonsterStateInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBBTTask_SetTurn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	StateInterface->SetCurrentState(MonsterState::Turn);
	MyCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

	return EBTNodeResult::Succeeded;
}
