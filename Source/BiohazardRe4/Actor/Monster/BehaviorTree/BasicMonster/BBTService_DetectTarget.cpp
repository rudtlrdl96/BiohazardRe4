// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BasicMonster/BBTService_DetectTarget.h"
#include "../../Interface/BMonsterStatInterface.h"
#include "AIController.h"
#include "../../Define/MonsterDefine.h"
#include "BiohazardRe4.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBBTService_DetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* OwnerCharacter = OwnerComp.GetAIOwner();
	if (OwnerCharacter == nullptr)
	{
		LOG_FATAL(TEXT("OwnerCharacter == nullptr : UBBTService_DetectTarget::TickNode"));
	}

	APawn* MyPawn = OwnerCharacter->GetPawn();
	if (MyPawn == nullptr)
	{
		LOG_FATAL(TEXT("MyPawn == nullptr : UBBTService_DetectTarget::TickNode"));
	}

	UWorld* CurWorld = MyPawn->GetWorld();
	if (CurWorld == nullptr)
	{
		LOG_FATAL(TEXT("CurWorld == nullptr : UBTService_CheckDistanceToTarget::TickNode"));
	}

	IBMonsterStatInterface* MyStatInterface = Cast<IBMonsterStatInterface>(MyPawn);
	if (MyStatInterface == nullptr)
	{
		LOG_FATAL(TEXT("MyStatInterface == nullptr : UBTService_CheckDistanceToTarget::TickNode"));
	}


	FVector MyLocation = MyPawn->GetActorLocation();
	float DetectRange = MyStatInterface->GetDetectRadius();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, MyPawn);

	bool bResult = CurWorld->OverlapMultiByChannel(
				   OverlapResults,
				   MyLocation,
				   FQuat::Identity,
				   ECC_EngineTraceChannel2,
				   FCollisionShape::MakeSphere(DetectRange),
				   CollisionQueryParam);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);

				break;
			}
		}
	}

}
