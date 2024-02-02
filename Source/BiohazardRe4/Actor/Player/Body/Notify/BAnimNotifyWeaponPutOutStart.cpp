// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/Notify/BAnimNotifyWeaponPutOutStart.h"
#include "BiohazardRe4.h"
#include "..\Interface\BInterface_WeaponPutOut.h"
#include "GameFramework/Character.h"

void UBAnimNotifyWeaponPutOutStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		ACharacter* Owner = Cast<ACharacter>(MeshComp->GetOwner());
		if (Owner == nullptr)
		{
			return;
		}

		IBInterface_WeaponPutOut* Interface = Cast<IBInterface_WeaponPutOut>(Owner);

		if (Interface == nullptr)
		{
			LOG_FATAL(TEXT("Interface Casting Fail"));
		}

		Interface->WeaponPutOutStart();
	}
}
