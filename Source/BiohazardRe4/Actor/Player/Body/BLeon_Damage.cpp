// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BLeon.h"
#include "Generic/BFsm.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "..\Weapon\BLeonWeapon.h"

void ABLeon::DamageEnter()
{
	bIsHitEnd = false;
	bIsPlayGetItem = false;
	bIsGunReload = false;
	bIsGunRecoil = false;

	if (nullptr != CurrentWeapon)
	{
		switch (UseWeaponCode)
		{
		case EItemCode::Handgun_SR09R: // Pistol
			LerpSocketEnd = TEXT("R_PistolSocket");
			break;
		case EItemCode::Shotgun_W870: // Shotgun
			LerpSocketEnd = TEXT("R_ShotgunSocket");
			break;
		case EItemCode::Rifle_SRM1903: // Rifle
			LerpSocketEnd = TEXT("R_RifleSocket");
			break;
		case EItemCode::CombatKnife: // Knife
			LerpSocketEnd = TEXT("R_KnifeSocket");
			bIsLerpWeaponChange = false;
			break;
		case EItemCode::Grenade: // Grenade
			LerpSocketEnd = TEXT("R_GrenadeSocket");
			break;
		case EItemCode::Flashbang: // Flashbang
			LerpSocketEnd = TEXT("R_FlashbangSocket");
			break;
		default:
		{
			const UEnum* EnupPtr = FindObject<UEnum>(nullptr, TEXT("/Script/BiohazardRe4.EItemCode"), true);

			FName CodeName;

			if (nullptr != EnupPtr)
			{
				CodeName = EnupPtr->GetNameByValue((int64)UseWeaponCode);
			}

			LOG_FATAL(TEXT("Wrong Item Code. Cannot be use as Weapons [%s]"), *CodeName.ToString());
		}
		break;
		}

		FVector EndLocation = GetMesh()->GetSocketLocation(LerpSocketEnd);
		FRotator EndRotation = GetMesh()->GetSocketRotation(LerpSocketEnd);

		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, LerpSocketEnd);

		CurrentWeapon->SetActorLocation(EndLocation);
		CurrentWeapon->SetActorRotation(EndRotation);
	}
}

void ABLeon::DamageUpdate(float _DeltaTime)
{
	if (true == bIsHitEnd)
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

void ABLeon::DamageExit()
{
	bIsHitEnd = false;
}