// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BTTask_ChainsawManWait.h"
#include "../Interface/BMonsterStateInterface.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BiohazardRe4.h"

void UBTTask_ChainsawManWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	IBMonsterStateInterface* StateInterface = Cast<IBMonsterStateInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (StateInterface == nullptr)
	{
		LOG_FATAL(TEXT("Monster : Interface casting failed : UBTTask_ChainsawManWait::TickTask - 15"));
		return;
	}

	LOG_MSG(TEXT("Monster : Chainsaw Man State Changed To Idle"));
}
