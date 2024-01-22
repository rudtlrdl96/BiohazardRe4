// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BBTService_DebugDrawing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Define/MonsterDefine.h"
#include "AIController.h"
#include "BiohazardRe4.h"

void UBBTService_DebugDrawing::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* MyController = OwnerComp.GetAIOwner();
	if (MyController == nullptr)
	{
		LOG_MSG(TEXT("1"));
		return;
	}

	APawn* MyPawn = MyController->GetPawn();
	if (MyPawn == nullptr)
	{
		LOG_MSG(TEXT("2"));
		return;
	}

	FVector MyLocation = MyPawn->GetActorLocation();

	FColor DebugColor = FColor::Green;
	FVector TargetLocation = FVector::Zero();

	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET);
	if (TargetObject != nullptr)
	{
		APawn* TargetPawn = Cast<APawn>(TargetObject);

		DebugColor = FColor::Red;
		TargetLocation = TargetPawn->GetActorLocation();
	}
	else
	{
		DebugColor = FColor::Blue;
		TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_PATROLPOS);
	}

	DrawDebugLine(OwnerComp.GetWorld(), MyLocation, TargetLocation, DebugColor, false, 0.5f);

}
