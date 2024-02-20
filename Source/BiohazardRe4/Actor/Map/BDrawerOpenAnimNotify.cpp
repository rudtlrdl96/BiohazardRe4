// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BDrawerOpenAnimNotify.h"
#include "Actor/Map/BMapDrawerInteraction.h"

void UBDrawerOpenAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		ABMapDrawerInteraction* Drawer = Cast<ABMapDrawerInteraction>(MeshComp->GetOwner());

		if (Drawer == nullptr)
		{
			return;
		}

		Drawer->SetDrawerValue(true);
	}
}
