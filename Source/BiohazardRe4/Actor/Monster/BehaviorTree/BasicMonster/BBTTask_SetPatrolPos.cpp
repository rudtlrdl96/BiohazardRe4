// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BasicMonster/BBTTask_SetPatrolPos.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BiohazardRe4.h"
#include "Actor/Monster/Define/MonsterDefine.h"
#include "Actor/Monster/Interface/BMonsterStateInterface.h"
#include "Actor/Monster/Interface/BMonsterStatInterface.h"

EBTNodeResult::Type UBBTTask_SetPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* MyPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (MyPawn == nullptr)
	{
		LOG_FATAL(TEXT("MyPawn == nullptr : UBBTTask_SetPatrolPos::ExecuteTask"));
		return EBTNodeResult::Failed;
	}
	
	ACharacter* MyCharacter = OwnerComp.GetAIOwner()->GetCharacter();
	if (MyCharacter == nullptr)
	{
		LOG_FATAL(TEXT("MyCharacter == nullptr : UBBTTask_SetPatrolPos::ExecuteTask"));
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(MyPawn->GetWorld());
	if (NavSystem == nullptr)
	{
		LOG_FATAL(TEXT("NavSystem == nullptr : UBBTTask_SetPatrolPos::ExecuteTask"));
		return EBTNodeResult::Failed;
	}

	IBMonsterStatInterface* StatInterface = Cast<IBMonsterStatInterface>(MyPawn);
	if (StatInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IBMonsterStateInterface* StateInterface = Cast<IBMonsterStateInterface>(MyPawn);
	if (StateInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}


	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
	float PatrolRadius = StatInterface->GetPatrolRadius();

	FNavLocation NextPatrolPos;
	if (NavSystem->GetRandomReachablePointInRadius(Origin, PatrolRadius, NextPatrolPos) != false)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);

		LOG_MSG(TEXT("StateChanged - Walk"));
		StateInterface->SetCurrentState(EMonsterState::Walk);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
