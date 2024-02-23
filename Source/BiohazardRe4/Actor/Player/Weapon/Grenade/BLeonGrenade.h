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

	UFUNCTION(BlueprintCallable)
	void SetFlashbang();

	UFUNCTION(BlueprintNativeEvent)
	void ExplosionEffect();
	virtual void ExplosionEffect_Implementation();

protected:

	UPROPERTY(EditAnywhere, Category = "Option")
	float LifeTime = 2.f;

	UPROPERTY(EditAnywhere, Category = "Option")
	float Radius = 450.f;

	float Time = 0.f;

	bool IsThrowing = false;
	
	TArray<AActor*, FDefaultAllocator> IgnoreDamageActor;

	EItemCode WeaponType = EItemCode::Grenade;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ExplosionSound = nullptr;

	void PlayExplosionSound();

};
