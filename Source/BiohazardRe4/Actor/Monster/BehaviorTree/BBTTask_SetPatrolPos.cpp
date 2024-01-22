// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BBTTask_SetPatrolPos.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BiohazardRe4.h"
#include "../Interface/BMonsterStatInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Define/MonsterDefine.h"

EBTNodeResult::Type UBBTTask_SetPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* MyPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (MyPawn == nullptr)
	{
		LOG_FATAL(TEXT("MyPawn == nullptr : UBBTTask_SetPatrolPos::ExecuteTask"));
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(MyPawn->GetWorld());
	if (NavSystem == nullptr)
	{
		LOG_FATAL(TEXT("NavSystem == nullptr : UBBTTask_SetPatrolPos::ExecuteTask"));
		return EBTNodeResult::Failed;
	}

	IBMonsterStatInterface* MyInterface = Cast<IBMonsterStatInterface>(MyPawn);
	if (MyInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_PATROLPOS);
	float PatrolRadius = MyInterface->GetPatrolRadius();

	FNavLocation NextPatrolPos;
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, PatrolRadius, NextPatrolPos) != false)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
