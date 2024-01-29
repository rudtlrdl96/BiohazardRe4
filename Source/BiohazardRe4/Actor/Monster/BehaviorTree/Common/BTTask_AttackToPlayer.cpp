// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/Common/BTTask_AttackToPlayer.h"
#include "../../Interface/BMonsterStateInterface.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Define/MonsterDefine.h"
#include "BiohazardRe4.h"

EBTNodeResult::Type UBTTask_AttackToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MyPawn)
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* MyCharacter = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if (nullptr == MyCharacter)
	{
		return EBTNodeResult::Failed;
	}

	IBMonsterStateInterface* StateInterface = Cast<IBMonsterStateInterface>(MyPawn);
	if (nullptr == StateInterface)
	{
		return EBTNodeResult::Failed;
	}

	FMonsterAttackEnd OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISNEAR, false);
			
			OwnerComp.GetAIOwner()->GetCharacter()->GetMesh()->GetAnimInstance()->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
		}
	);

	MyCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	StateInterface->SetMonsterAttackEndDelegate(OnAttackFinished);
	StateInterface->SetCurrentState(EMonsterState::Attack);

	return EBTNodeResult::InProgress;
}
