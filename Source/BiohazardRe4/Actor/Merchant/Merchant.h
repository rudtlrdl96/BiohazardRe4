// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Merchant.generated.h"

UENUM(BlueprintType)
enum class EMerchant : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Open			UMETA(DisplayName = "Open"),
	GiveHand		UMETA(DisplayName = "GiveHand"),
};

UCLASS()
class BIOHAZARDRE4_API ABMerchant : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABMerchant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
