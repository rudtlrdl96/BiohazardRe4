// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Player/Weapon/BLeonWeapon.h"
#include "BLeonGrenade.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABLeonGrenade : public ABLeonWeapon
{
	GENERATED_BODY()
	
public:
	ABLeonGrenade();

	virtual void Tick(float DeltaTime) override;
	
	virtual void Explosion();

	virtual void Attack() override;

protected:

	UPROPERTY(EditAnywhere, Category = "Option")
	float LifeTime = 3.f;

	UPROPERTY(EditAnywhere, Category = "Option")
	float Radius = 300.f;

	UPROPERTY(EditAnywhere, Category = "Option")
	float Damage = 100.f;

	float Time = 0.f;

	bool IsThrowing = false;
	
	TArray<AActor*, FDefaultAllocator> IgnoreDamageActor;

};
