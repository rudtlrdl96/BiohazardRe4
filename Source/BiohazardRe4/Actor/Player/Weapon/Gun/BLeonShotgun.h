// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Player/Weapon/Gun/BLeonGun.h"
#include "BLeonShotgun.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABLeonShotgun : public ABLeonGun
{
	GENERATED_BODY()
public:
	ABLeonShotgun();
		
protected:
	virtual void BeginPlay() override;

	virtual void Reload() override;
	virtual void ReloadEnd() override;
	virtual void ReloadEndAfter() override;
};
