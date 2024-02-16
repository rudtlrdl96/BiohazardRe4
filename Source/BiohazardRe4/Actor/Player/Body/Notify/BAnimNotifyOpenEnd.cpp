// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/Notify/BAnimNotifyOpenEnd.h"
#include "BiohazardRe4.h"
#include "../Interface/BIPlayerInteractionAnim.h"
#include "GameFramework/Character.h"

void UBAnimNotifyOpenEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		ACharacter* Owner = Cast<ACharacter>(MeshComp->GetOwner());
		if (Owner == nullptr)
		{
			return;
		}

		IBIPlayerInteractionAnim* Interface = Cast<IBIPlayerInteractionAnim>(Owner);

		if (Interface == nullptr)
		{
			LOG_FATAL(TEXT("Interface Casting Fail"));
		}

		Interface->DoorOpenEnd();
	}
}
