// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BTTaskNode_FindNextPatrolPos.h"
#include "../Define/MonsterDefine.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Interface/BMonsterStateInterface.h"

UBTTaskNode_FindNextPatrolPos::UBTTaskNode_FindNextPatrolPos()
{
}

EBTNodeResult::Type UBTTaskNode_FindNextPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
	FNavLocation NextPatrolPos;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 600.0f, NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		
		IBMonsterStateInterface* OwnerStateInterface = Cast<IBMonsterStateInterface>(ControllingPawn);
		
		if (OwnerStateInterface != nullptr)
		{
			OwnerStateInterface->SetCurrentState(MonsterState::Run);
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
