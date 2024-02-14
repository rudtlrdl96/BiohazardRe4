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

	virtual void PostActorCreated() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	virtual void Explosion();
	virtual void ExplosionAttack(AActor* _OverlapActor);

	virtual void Attack() override;

private:

	UPROPERTY(EditAnywhere, Category = "Option")
	float LifeTime = 3.f;

	float Time = 0.f;

	ABCollisionObserverSphere* ExplosionCollision = nullptr;
	
	bool IsThrowing = false;

	float Radius = 100.f;

	TArray<AActor*, FDefaultAllocator> IgnoreDamage;

};
