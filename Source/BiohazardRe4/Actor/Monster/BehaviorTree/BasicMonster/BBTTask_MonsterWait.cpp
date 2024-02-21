// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BasicMonster/BBTTask_MonsterWait.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BiohazardRe4.h"
#include "Actor/Monster/Interface/BMonsterStateInterface.h"

UBBTTask_MonsterWait::UBBTTask_MonsterWait()
{
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBBTTask_MonsterWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACharacter* MyCharacter = OwnerComp.GetAIOwner()->GetCharacter();
	if (MyCharacter == nullptr)
	{
		LOG_FATAL(TEXT("MyCharacter == nullptr : UBBTTask_SetPatrolPos::ExecuteTask"));
		return EBTNodeResult::Failed;
	}

	IBMonsterStateInterface* MyInterface = Cast<IBMonsterStateInterface>(MyCharacter);
	if (MyInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	} 

	MyInterface->SetCurrentState(EMonsterState::Idle);

	return EBTNodeResult::InProgress;
}

void UBBTTask_MonsterWait::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	ACharacter* MyCharacter = OwnerComp.GetAIOwner()->GetCharacter();
	if (MyCharacter == nullptr)
	{
		LOG_WARNING(TEXT("MyCharacter == nullptr : UBBTTask_SetPatrolPos::ExecuteTask"));
		return;
	}

	IBMonsterStateInterface* MyInterface = Cast<IBMonsterStateInterface>(MyCharacter);
	if (MyInterface == nullptr)
	{
		return;
	}
	
	if (MyInterface->GetCurrentState() == EMonsterState::Idle)
	{
		MyInterface->SetCurrentState(EMonsterState::Walk);
	}
}
