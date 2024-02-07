// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BInteraction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBInteraction : public UInterface
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EInteraction
{
	Empty			UMETA(DisplayName = "Empty"),
	AttackMonster	UMETA(DisplayName = "AttackMonster"),
	GroggyMonster	UMETA(DisplayName = "GroggyMonster"),
	JumpWindow		UMETA(DisplayName = "JumpWindow"),
	FallCliff		UMETA(DisplayName = "FallCliff"),
	JumpFence		UMETA(DisplayName = "JumpFence"),
	OpenDoor		UMETA(DisplayName = "OpenDoor"),
	DropItem		UMETA(DisplayName = "DropItem"),
};

/**
 * 
 */
class BIOHAZARDRE4_API IBInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool AbleGroggy() const = 0;
	virtual EInteraction GetInteractionType() const = 0;
	virtual FVector GetUIPivot() const = 0;
};