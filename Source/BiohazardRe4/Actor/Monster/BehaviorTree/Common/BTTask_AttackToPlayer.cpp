// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackToPlayer.h"
#include "AIController.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BiohazardRe4.h"
#include "Actor/Monster/Define/MonsterDefine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/Monster/Interface/BMonsterStateInterface.h"


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

	USkeletalMeshComponent* MyMesh = MyCharacter->GetMesh();
	if (MyMesh == nullptr)
	{
		//LOG_MSG(TEXT("MyMesh is nullptr"));
		return EBTNodeResult::Failed;
	}

	UAnimInstance* Anim = MyMesh->GetAnimInstance();
	if (Anim == nullptr)
	{
		//LOG_MSG(TEXT("Anim is nullptr"));
		return EBTNodeResult::Failed;
	}

	FMonsterAttackEnd OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&, Anim]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISNEAR, false);
			Anim->StopAllMontages(1.0f);
		}
	);
	
	MyCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	
	StateInterface->SetMonsterAttackEndDelegate(OnAttackFinished);
	StateInterface->AttackStart();

	return EBTNodeResult::InProgress;
}
