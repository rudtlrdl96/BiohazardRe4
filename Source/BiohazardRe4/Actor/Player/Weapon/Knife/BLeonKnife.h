// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Player/Weapon/BLeonWeapon.h"
#include "Sound/SoundCue.h"
#include "BLeonKnife.generated.h"

class ABCollisionObserverCapsule;

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABLeonKnife : public ABLeonWeapon
{
	GENERATED_BODY()
	
public:	
	virtual void BeginPlay() override;

	virtual void ActiveCollision(bool _IsActive) override;

private:
	uint32 bCollisionActive : 1 = false;
	uint32 bIsKnifeAttackSoundPlay : 1 = false;

	ABCollisionObserverCapsule* AttackCollision = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	USoundCue* KnifeHitSound = nullptr;

	void KnifeAttack(AActor* _OverlapActor);


};
