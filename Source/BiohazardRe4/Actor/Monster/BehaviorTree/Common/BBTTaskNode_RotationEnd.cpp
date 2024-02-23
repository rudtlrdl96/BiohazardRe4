// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/Common/BBTTaskNode_RotationEnd.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/Monster/Interface/BMonsterStateInterface.h"
#include "BiohazardRe4.h"

EBTNodeResult::Type UBBTTaskNode_RotationEnd::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (MyController == nullptr)
	{
		LOG_MSG(TEXT("Controller is nullptr"));
		return EBTNodeResult::Failed;
	}

	APawn* MyPawn = Cast<APawn>(MyController->GetPawn());
	if (MyPawn == nullptr)
	{
		LOG_MSG(TEXT("Pawn is nullptr"));
		return EBTNodeResult::Failed;
	}

	ACharacter* MyCharacter = Cast<ACharacter>(MyPawn);
	if (MyCharacter == nullptr)
	{
		LOG_MSG(TEXT("Character is nullptr"));
		return EBTNodeResult::Failed;
	}

	IBMonsterStateInterface* StateInterface = Cast<IBMonsterStateInterface>(MyPawn);
	if (StateInterface == nullptr)
	{
		LOG_MSG(TEXT("StateInterface is nullptr"));
		return EBTNodeResult::Failed;
	}

	MyCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	return EBTNodeResult::Succeeded;
}
