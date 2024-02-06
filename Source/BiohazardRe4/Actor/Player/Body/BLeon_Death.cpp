// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BLeon.h"
#include "Generic/BFsm.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "..\Weapon\BLeonWeapon.h"

void ABLeon::DeathEnter()
{
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		FVector ThrowVector = GetActorForwardVector() + FVector::UpVector;
		ThrowVector.Normalize();

		CurrentWeapon->ActiveSimulatePhysics(true);
	}
}

void ABLeon::DeathUpdate(float _DeltaTime)
{

}

void ABLeon::DeathExit()
{

}