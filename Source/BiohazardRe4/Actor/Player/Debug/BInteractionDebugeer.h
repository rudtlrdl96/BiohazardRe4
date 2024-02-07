// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Actor/Generic/Interface/BInteraction.h"
#include "BInteractionDebugeer.generated.h"

UCLASS()
class BIOHAZARDRE4_API ABInteractionDebugeer : public AActor, public IBInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABInteractionDebugeer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool AbleGroggy() const override
	{
		return bIsActiveInteraction;
	}

	virtual EInteraction GetInteractionType() const override
	{
		return TempInterType;
	}

	virtual FVector GetUIPivot() const override
	{
		return Pivot;
	}

private:
	UPROPERTY(EditAnywhere)
	EInteraction TempInterType;

	UPROPERTY(EditAnywhere)
	uint8 bIsActiveInteraction : 1 = false;

	UPROPERTY(EditAnywhere)
	FVector Pivot = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	USphereComponent* Collision = nullptr;
};
