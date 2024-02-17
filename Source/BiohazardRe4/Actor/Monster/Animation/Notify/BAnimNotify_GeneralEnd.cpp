// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/Animation/Notify/BAnimNotify_GeneralEnd.h"
#include "Actor/Monster/Interface/BMonsterStateInterface.h"
#include "BiohazardRe4.h"

void UBAnimNotify_GeneralEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();
	if (OwnerActor == nullptr)
	{
		return;
	}

	IBMonsterStateInterface* StateInterface = Cast<IBMonsterStateInterface>(OwnerActor);
	if (StateInterface == nullptr)
	{
		LOG_WARNING(TEXT("Interface Casting failed"));
		return;
	}

	StateInterface->SetCurrentState(EMonsterState::Walk);
}
