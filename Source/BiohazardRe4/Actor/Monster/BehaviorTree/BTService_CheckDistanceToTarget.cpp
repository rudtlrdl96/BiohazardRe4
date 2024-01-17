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

UBTService_CheckDistanceToTarget::UBTService_CheckDistanceToTarget()
{
	NodeName = TEXT("CheckDistanceToTarget");
	Interval = 1.0f;
}

void UBTService_CheckDistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* MyPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (MyPawn == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("MyPawn == nullptr, / CheckDistanceToTarget : Line 27"));
	}

	UWorld* CurWorld = MyPawn->GetWorld();
	FVector MyLocation = MyPawn->GetActorLocation();
	float SearchRadius = 600.0f;

	if(CurWorld == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("CurWorld == nullptr, / CheckDistanceToTarget : Line 36"));
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, MyPawn);

	bool bResult = CurWorld->OverlapMultiByChannel(OverlapResults, MyLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, 
													FCollisionShape::MakeSphere(SearchRadius), QueryParams);

	if (bResult == true)
	{
		for (FOverlapResult OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn != nullptr && Pawn->GetController()->IsPlayerController() == true)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISNEAR, true);
				
				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISNEAR, false);
	}
}
