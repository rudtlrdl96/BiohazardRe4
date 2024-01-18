// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BTService_CheckDistanceToTarget.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "../Define/MonsterDefine.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "BiohazardRe4.h"

UBTService_CheckDistanceToTarget::UBTService_CheckDistanceToTarget()
{
	NodeName = TEXT("CheckDistanceToTarget");
	Interval = 0.25f;
}

void UBTService_CheckDistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* MyPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (MyPawn == nullptr)
	{
		LOG_FATAL(TEXT("MyPawn == nullptr : UBTService_CheckDistanceToTarget::TickNode - 26"));
		return;
	}

	UWorld* CurWorld = MyPawn->GetWorld();
	if(CurWorld == nullptr)
	{
		LOG_FATAL(TEXT("CurWorld == nullptr : UBTService_CheckDistanceToTarget::TickNode - 32"));
		return;
	}

	FVector MyLocation = MyPawn->GetActorLocation();
	float SearchRadius = 100.0f;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, MyPawn);

	bool bResult = CurWorld->OverlapMultiByChannel(OverlapResults, MyLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, 
												   FCollisionShape::MakeSphere(SearchRadius), QueryParams);

	if (bResult == true)
	{
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn != nullptr && Pawn->GetController()->IsPlayerController() == true)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISNEAR, true);	
				LOG_MSG(TEXT("Monster : Player is in AttackRange of ChainsawMan"));
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISNEAR, false);
	LOG_MSG(TEXT("Monster : Player is out AttackRange of ChainsawMan"));
}
