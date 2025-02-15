// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BLeon.h"
#include "Generic/BFsm.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "..\Weapon\BLeonWeapon.h"
#include "BiohazardRe4.h"
#include "Actor/Player/HUD/HUDBase.h"
#include "..\Weapon\BDrawGrenadeAim.h"

void ABLeon::AimEnter()
{
	bIsJog = false;
	bIsCrouch = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	AimUpdateTime = 0.0f;
	LeonAim = ELeonAim::Start;
	bDrawGrenadeAim = false;
	bIsPlayGetItem = false;

	ELeonWeaponAnim WeaponAnim = GetUseWeaponAnimation(UseWeaponCode);

	if (ELeonWeaponAnim::Grenade != WeaponAnim)
	{
		HUD->SetCrosshair(true);
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), AimActiveVoiceSound, GetActorLocation());

	switch (UseWeaponCode)
	{
	case EItemCode::Handgun_SR09R: // Pistol
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PistolAimActiveSound, GetActorLocation());
	}
		break;
	case EItemCode::Shotgun_W870: // Shotgun
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShotgunAimActiveSound, GetActorLocation());
	}
		break;
	case EItemCode::Rifle_SRM1903: // Rifle
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), RifleAimActiveSound, GetActorLocation());
	}
		break;
	case EItemCode::Grenade: // Grenade
	case EItemCode::Flashbang: // FlashBang
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GrenadeAimActiveSound, GetActorLocation());
	}
		break;
	}
}

void ABLeon::AimUpdate(float _DeltaTime)
{
	bIsCrouch = false;

	MoveDir = FMath::VInterpConstantTo(MoveDir, MoveInput, _DeltaTime, 6.0f);
	Aim(_DeltaTime);

	ELeonWeaponAnim WeaponAnim = GetUseWeaponAnimation(UseWeaponCode);

	if (ELeonWeaponAnim::Grenade == WeaponAnim)
	{
		bDrawGrenadeAim = true;
		DrawGrenadeAim(_DeltaTime);
	}

	if (true == bIsGunRecoil)
	{
		AimUpdateTime = FMath::FInterpConstantTo(AimUpdateTime, 1.0f, _DeltaTime, 3.0f);
		return;
	}

	if (true == bIsWeaponShootTrigger)
	{
		bIsWeaponShootTrigger = false;

		if (true == CurrentWeapon->AbleAttack())
		{
			switch (WeaponAnim)
			{
			// Gun 
			case ELeonWeaponAnim::Pistol:
			{
				GunRecoilType = EGunRecoilType::Pistol;
				CurrentWeapon->Attack();
				bIsGunRecoil = true;
				bIsCameraRecoil = true;
				GunRecoilAlpha = 0.0f;
			}
			break;
			case ELeonWeaponAnim::Shotgun:
			{
				GunRecoilType = EGunRecoilType::Shotgun;
				CurrentWeapon->Attack();
				bIsGunRecoil = true;
				bIsCameraRecoil = true;
				GunRecoilAlpha = 0.0f;
			}
			break;
			case ELeonWeaponAnim::Rifle:
			{
				GunRecoilType = EGunRecoilType::Rifle;
				CurrentWeapon->Attack();
				bIsGunRecoil = true;
				bIsCameraRecoil = true;
				GunRecoilAlpha = 0.0f;
			}
			break;
			// Knife
			case ELeonWeaponAnim::Knife:
			{

			}
			break;
			// Grenade
			case ELeonWeaponAnim::Grenade:
			{
				//CurrentWeapon->Attack();
				FsmComp->ChangeState(TO_KEY(ELeonState::Throwing));
			}
			break;
			default:
				break;
			}
		}
	}

	if (true == bIsGunReload)
	{
		bIsAim = false;
		AimUpdateTime = 0.0f;
	}

	if (LeonWeaponSwap != ELeonWeaponSwap::None)
	{
		bIsAim = false;
		AimUpdateTime = 0.0f;
	}

	if (true == bIsAim)
	{
		AimUpdateTime = FMath::FInterpConstantTo(AimUpdateTime, 1.0f, _DeltaTime, 3.0f);

		if (1.0f <= AimUpdateTime)
		{
			LeonAim = ELeonAim::Loop;
		}
		else
		{
			LeonAim = ELeonAim::Start;
		}
	}
	else
	{		
		AimUpdateTime = FMath::FInterpConstantTo(AimUpdateTime, 0.0f, _DeltaTime, 5.0f);

		LeonAim = ELeonAim::End;

		if (0.0 < AimUpdateTime)
		{
			return;
		}

		if (MoveInput == FVector::ZeroVector)
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

void ABLeon::AimExit()
{
	switch (UseWeaponCode)
	{
	case EItemCode::Handgun_SR09R: // Pistol
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PistolAimDisableSound, GetActorLocation());
	}
	break;
	case EItemCode::Rifle_SRM1903: // Rifle
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), RifleAimDisableSound, GetActorLocation());
	}
	break;
	}

	LeonAim = ELeonAim::Start;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bDrawGrenadeAim = false;
	GrenadeAimActor->DisableDraw();
	HUD->SetCrosshair(false);
}