// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BAnimNotifyWeaponReload.h"
#include "BiohazardRe4.h"
#include "BInterface_WeaponReload.h"
#include "GameFramework/Character.h"

void UBAnimNotifyWeaponReload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		ACharacter* Owner = Cast<ACharacter>(MeshComp->GetOwner());
		if (Owner == nullptr)
		{
			return;
		}

		IBInterface_WeaponReload* Interface = Cast<IBInterface_WeaponReload>(Owner);

		if (Interface == nullptr)
		{
			LOG_FATAL(TEXT("Interface Casting Fail"));
		}

		Interface->Reload();

		if (true != Interface->AbleReload())
		{
			UAnimInstance* Anim = MeshComp->GetAnimInstance();

			if (nullptr == Anim)
			{
				return;
			}

			UAnimMontage* Montage = Anim->GetCurrentActiveMontage();

			if (nullptr == Montage)
			{
				return;
			}

			int32 EndIndex = Montage->GetSectionIndex("End");

			if (INDEX_NONE == EndIndex)
			{
				return;
			}

			Anim->Montage_JumpToSection("End", Montage);
		}
	}
}