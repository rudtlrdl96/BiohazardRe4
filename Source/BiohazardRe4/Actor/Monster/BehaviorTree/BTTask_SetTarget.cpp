// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BTTask_SetTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Define/MonsterDefine.h"
#include "../Interface/BMonsterStateInterface.h"

EBTNodeResult::Type UBTTask_SetTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APlayerController* PlayerPtr = OwnerComp.GetWorld()->GetFirstPlayerController();
	APawn* TargetPtr = PlayerPtr->GetPawn();

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, TargetPtr);

	return EBTNodeResult::Succeeded;
}
