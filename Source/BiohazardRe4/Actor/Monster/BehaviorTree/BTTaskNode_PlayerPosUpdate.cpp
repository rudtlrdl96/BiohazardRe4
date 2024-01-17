// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BTTaskNode_PlayerPosUpdate.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Define/MonsterDefine.h"
#include "../Interface/BMonsterStateInterface.h"

EBTNodeResult::Type UBTTaskNode_PlayerPosUpdate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AActor* PlayerPtr = GetWorld()->GetFirstPlayerController();
	FVector TargetPos = PlayerPtr->GetTransform().GetLocation();

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PLAYERPOS, TargetPos);

	return EBTNodeResult::Succeeded;
}
