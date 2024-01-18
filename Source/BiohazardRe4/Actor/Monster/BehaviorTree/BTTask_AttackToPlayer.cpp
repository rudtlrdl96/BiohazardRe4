// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BTTask_AttackToPlayer.h"
#include "../Interface/BMonsterStateInterface.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Define/MonsterDefine.h"
#include "BiohazardRe4.h"

EBTNodeResult::Type UBTTask_AttackToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerAI = OwnerComp.GetAIOwner();
	if (OwnerAI == nullptr)
	{
		LOG_FATAL(TEXT("OwnerAI == nullptr : UBTTask_AttackToPlayer::ExecuteTask - 15"));
		return EBTNodeResult::Failed;
	}

	APawn* OwnerPawn = OwnerAI->GetPawn();
	if (OwnerPawn == nullptr)
	{
		LOG_FATAL(TEXT("OwnerPawn == nullptr : UBTTask_AttackToPlayer::ExecuteTask - 22"));
		return EBTNodeResult::Failed;
	}


	IBMonsterStateInterface* StateInterface = Cast<IBMonsterStateInterface>(OwnerPawn);
	if (StateInterface == nullptr)
	{
		LOG_FATAL(TEXT("Interface Casting Failed : UBTTask_AttackToPlayer::ExecuteTask - 30"));
		return EBTNodeResult::Failed;
	}

	StateInterface->SetCurrentState(MonsterState::Attack);
	StateInterface->SetIsAttack(true);

	OwnerComp.GetAIOwner()->GetCharacter()->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISATTACKING, StateInterface->IsAttacking());
	
	LOG_MSG(TEXT("Monster Attack To Player"));


	return EBTNodeResult::Succeeded;
}
