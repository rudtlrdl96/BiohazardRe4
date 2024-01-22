// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BBTService_RangeCheck.h"
#include "BiohazardRe4.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "../Interface/BMonsterStatInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Define/MonsterDefine.h"

void UBBTService_RangeCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISNEAR) == true)
	{
		return;
	}

	APawn* MyPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (MyPawn == nullptr)
	{
		LOG_FATAL(TEXT("MyPawn == nullptr : UBTService_CheckDistanceToTarget::TickNode"));
	}

	ACharacter* MyCharacter = OwnerComp.GetAIOwner()->GetCharacter();
	if (MyCharacter == nullptr)
	{
		LOG_FATAL(TEXT("MyCharacter == nullptr : UBTService_CheckDistanceToTarget::TickNode"));
	}

	UWorld* CurWorld = MyPawn->GetWorld();
	if (CurWorld == nullptr)
	{
		LOG_FATAL(TEXT("CurWorld == nullptr : UBTService_CheckDistanceToTarget::TickNode"));
	}

	IBMonsterStatInterface* MyStatInterface = Cast<IBMonsterStatInterface>(MyCharacter);
	if (MyStatInterface == nullptr)
	{
		LOG_FATAL(TEXT("MyStatInterface == nullptr : UBTService_CheckDistanceToTarget::TickNode"));
	}

	FVector MyLocation = MyCharacter->GetActorLocation();
	float AttackRange = MyStatInterface->GetAttackRange();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, MyPawn);
	
	bool bResult = CurWorld->OverlapMultiByChannel(
				   OverlapResults,
				   MyLocation,
				   FQuat::Identity,
				   ECC_EngineTraceChannel2,
				   FCollisionShape::MakeSphere(AttackRange),
				   CollisionQueryParam);


	FColor DebugColor = FColor::Green;
	bool bIsFind = false;

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				DebugColor = FColor::Red;
				bIsFind = true;
				break;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISNEAR, bIsFind);

#if ENABLE_DRAW_DEBUG
	DrawDebugSphere(CurWorld, MyLocation, AttackRange, 16, DebugColor, false, 0.2f);
#endif

}
