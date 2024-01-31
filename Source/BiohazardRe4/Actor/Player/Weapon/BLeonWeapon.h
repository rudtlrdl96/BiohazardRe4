// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "BiohazardRe4.h"
#include "BLeonWeapon.generated.h"

UCLASS()
class BIOHAZARDRE4_API ABLeonWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABLeonWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned	
	virtual void BeginPlay() override;

public:
	virtual void Trigger();
	virtual void Reload();

	virtual void Attack();
	bool AbleAttack();

	FTransform GetLeftHandSocketTransform() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Mesh")
	USkeletalMeshComponent* WeaponMesh = nullptr;

};