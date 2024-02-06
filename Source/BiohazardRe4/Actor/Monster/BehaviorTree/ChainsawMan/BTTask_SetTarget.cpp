// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/ChainsawMan/BTTask_SetTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Define/MonsterDefine.h"
#include "../../Interface/BMonsterStateInterface.h"
#include "BiohazardRe4.h"
#include "GameFramework/Character.h"
#include "../../Interface/BMonsterAnimInterface.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_SetTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APlayerController* PlayerPtr = OwnerComp.GetWorld()->GetFirstPlayerController();
	APawn* TargetPtr = PlayerPtr->GetPawn();

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, TargetPtr);

	ACharacter* OwnerCharacter = OwnerComp.GetAIOwner()->GetCharacter();
	if (OwnerCharacter == nullptr)
	{
		LOG_WARNING(TEXT("OwnerCharacter is nullptr"));
		return EBTNodeResult::Failed;
	}

	UAnimInstance* OwnerAnim = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (OwnerAnim == nullptr)
	{
		LOG_WARNING(TEXT("OwnerAnim is nullptr"));
		return EBTNodeResult::Failed;
	}

	IBMonsterAnimInterface* AnimInterface = Cast<IBMonsterAnimInterface>(OwnerAnim);
	if (AnimInterface == nullptr)
	{
		LOG_WARNING(TEXT("AnimInterface is nullptr"));
		return EBTNodeResult::Failed;
	}

	AnimInterface->SetTarget(TargetPtr);

	return EBTNodeResult::Succeeded;
}
