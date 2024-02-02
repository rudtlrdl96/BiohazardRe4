// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Player/Weapon/BLeonWeapon.h"
#include "BLeonGun.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABLeonGun : public ABLeonWeapon
{
	GENERATED_BODY()

public:
	virtual bool AbleAttack() const override;
	virtual void Attack() override;

	virtual bool AbleReload() const override;
	virtual void Reload() override;

protected:
	virtual void BeginPlay() override;

	virtual void Shoot();
	virtual void DropShell();
	virtual void DropMagazine();


	//  ______________________LineTraceOption
	UPROPERTY()
	float Range = 5000.f;

	UPROPERTY()
	APlayerCameraManager* PlayerCamManager = nullptr;

	UPROPERTY()
	FHitResult HitInfo;

private:
	UPROPERTY()
	FVector LineTraceStart = FVector::Zero();

	UPROPERTY()
	FVector LineTraceEnd = FVector::Zero();

};
