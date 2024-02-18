// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BOpenDoorEndAnimNotify.h"
#include "Actor/Map/BMapDoorInteraction.h"

void UBOpenDoorEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		ABMapDoorInteraction* Door = Cast<ABMapDoorInteraction>(MeshComp->GetOwner());

		if (Door == nullptr)
		{
			return;
		}

		//Owner->IsSetOpen(false);
		Door->IsSetFastOpen(false);
		Door->IsSetCloseValue(false);
	}
}