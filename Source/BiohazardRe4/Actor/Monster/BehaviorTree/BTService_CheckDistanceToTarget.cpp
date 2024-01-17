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

	UObject* TargetPtr = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET);
	APawn* CastedTargetPtr = Cast<APawn>(TargetPtr);

	FVector TargetLocation = CastedTargetPtr->GetActorLocation();
	ACharacter* MyCastedPtr = Cast<ACharacter>(OwnerComp.GetOwner());

	//if (Distance < 500.0f)
	//{
	//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISNEAR, true);
	//}
	//else
	//{
	//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISNEAR, false);
	//}
}
