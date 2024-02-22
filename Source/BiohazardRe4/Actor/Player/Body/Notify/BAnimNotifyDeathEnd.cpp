// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/Notify/BAnimNotifyDeathEnd.h"
#include "BiohazardRe4.h"
#include "..\Interface\BIDamage.h"
#include "GameFramework/Character.h"

void UBAnimNotifyDeathEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		ACharacter* Owner = Cast<ACharacter>(MeshComp->GetOwner());
		if (Owner == nullptr)
		{
			return;
		}

		IBIDamage* Interface = Cast<IBIDamage>(Owner);

		if (Interface == nullptr) 
		{
			LOG_FATAL(TEXT("Interface Casting Fail"));
		}

		Interface->DeathEnd();
	}
}
