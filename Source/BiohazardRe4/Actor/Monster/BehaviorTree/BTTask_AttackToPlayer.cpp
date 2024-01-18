// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BTTask_AttackToPlayer.h"
#include "../Interface/BMonsterStateInterface.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Define/MonsterDefine.h"

EBTNodeResult::Type UBTTask_AttackToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	IBMonsterStateInterface* StateInterface = Cast<IBMonsterStateInterface>(OwnerComp.GetAIOwner()->GetPawn());

	if (StateInterface != nullptr)
	{
		StateInterface->SetCurrentState(MonsterState::Attack);
		StateInterface->SetIsAttack(true);

		OwnerComp.GetAIOwner()->GetCharacter()->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISATTACKING, StateInterface->IsAttacking());

		UE_LOG(LogTemp, Log, TEXT("Monster : Attack To Player"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Interface Casting Failed : UBTTask_AttackToPlayer::ExecuteTask - 18"));
	}

	return EBTNodeResult::Succeeded;
}
