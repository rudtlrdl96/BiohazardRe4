// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BLeon.h"
#include "Generic/BFsm.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Item/InventoryManager.h"
#include "BiohazardRe4.h"
#include "..\Weapon\BLeonWeapon.h"

void ABLeon::ThrowingEnter()
{
	bIsThrowingEnd = false;
	bIsThrowingWeapon = false;
	bIsPlayGetItem = false;
}

void ABLeon::ThrowingUpdate(float _DeltaTime)
{
	if (true == bIsThrowingWeapon)
	{
		if (nullptr == CurrentWeapon)
		{
			LOG_FATAL(TEXT("Player Weapon is nullptr"));
		}

		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Attack();
		CurrentWeapon->SetActorLocation(ThrowLocation);
		CurrentWeapon->ThrowWeapon(ThrowVelocity);
		CurrentWeapon = nullptr;
		bIsThrowingWeapon = false;

		if (nullptr == UBInventoryManager::Instance || 0 >= UBInventoryManager::Instance->GetItemCount(UseWeaponCode))
		{
			UseWeaponCode = EItemCode::Empty;
		}
	}

	if (true == bIsThrowingEnd)
	{
		if (true == AbleAim() && true == bIsAim)
		{
			FsmComp->ChangeState(TO_KEY(ELeonState::Aim));
		}
		else if (MoveInput == FVector::ZeroVector)
		{
			FsmComp->ChangeState(TO_KEY(ELeonState::Idle));
		}
		else if (true == bIsJogTrigger)
		{
			FsmComp->ChangeState(TO_KEY(ELeonState::Jog));
		}
		else
		{
			FsmComp->ChangeState(TO_KEY(ELeonState::Walk));
		}
	}
}

void ABLeon::ThrowingExit()
{
	bIsThrowingEnd = false;
	bIsThrowingWeapon = false;

	if (UseWeaponCode == EItemCode::Empty)
	{
		return;
	}

	if (nullptr == UBInventoryManager::Instance || 0 >= UBInventoryManager::Instance->GetItemCount(UseWeaponCode))
	{
		UseWeaponCode = EItemCode::Empty;
		CurrentWeapon = nullptr;
		return;
	}

	CurrentWeapon = CreateWeapon(UseWeaponCode);

	FVector EndLocation = GetMesh()->GetSocketLocation(LerpSocketEnd);
	FRotator EndRotation = GetMesh()->GetSocketRotation(LerpSocketEnd);

	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, LerpSocketEnd);

	CurrentWeapon->SetActorLocation(EndLocation);
	CurrentWeapon->SetActorRotation(EndRotation);
}