// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/WeaponAnimation/Notify/BWAnimNotifyPutoutEnd.h"
#include "Actor/Player/Weapon/Gun/BLeonGun.h"

void UBWAnimNotifyPutoutEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		ABLeonGun* Owner = Cast<ABLeonGun>(MeshComp->GetOwner());
		if (Owner == nullptr)
		{
			return;
		}

		Owner->PutoutEnd();
	}
}
