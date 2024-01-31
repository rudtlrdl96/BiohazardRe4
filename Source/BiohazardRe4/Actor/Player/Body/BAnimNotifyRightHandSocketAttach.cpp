// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BAnimNotifyRightHandSocketAttach.h"
#include "BiohazardRe4.h"
#include "BInterface_WeaponHandSocketSwap.h"
#include "GameFramework/Character.h"

void UBAnimNotifyRightHandSocketAttach::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		ACharacter* Owner = Cast<ACharacter>(MeshComp->GetOwner());
		if (Owner == nullptr)
		{
			return;
		}

		IBInterface_WeaponHandSocketSwap* Interface = Cast<IBInterface_WeaponHandSocketSwap>(Owner);

		if (Interface == nullptr)
		{
			LOG_FATAL(TEXT("Interface Casting Fail"));
		}

		Interface->AttachRightHandSocket();
	}
}