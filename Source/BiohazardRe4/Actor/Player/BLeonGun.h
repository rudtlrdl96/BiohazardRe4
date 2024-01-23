// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BLeonGun.generated.h"

UCLASS()
class BIOHAZARDRE4_API ABLeonGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABLeonGun();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Trigger();
	virtual void Shoot();
	virtual void Reload();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
