// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BCloseEndDoorAnimNotify.h"
#include "Actor/Map/BMapDoorInteraction.h"
void UBCloseEndDoorAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		ABMapDoorInteraction* Owner = Cast<ABMapDoorInteraction>(MeshComp->GetOwner());
		if (Owner == nullptr)
		{
			return;
		}
		Owner->IsSetOpen(false);
		Owner->IsSetFastOpen(false);
		Owner->IsSetCloseValue(true);
	}
}
