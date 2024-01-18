// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BTTaskNode_CheckAttackEnd.h"
#include "../Interface/BMonsterStateInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "../Define/MonsterDefine.h"

EBTNodeResult::Type UBTTaskNode_CheckAttackEnd::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	IBMonsterStateInterface* StateInterface = Cast<IBMonsterStateInterface>(OwnerComp.GetAIOwner()->GetPawn());

	bool IsAttacking = StateInterface->IsAttacking();

	if (StateInterface != nullptr)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISATTACKING, IsAttacking);
	}

	if (IsAttacking == true)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		return EBTNodeResult::Succeeded;
	}
}
