// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Map/BMapBaseInteraction.h"
#include "BMapDoorInteraction.generated.h"

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	None	UMETA(DisplayName = "None"),
	Front	UMETA(DisplayName = "Front"),
	Back	UMETA(DisplayName = "Back"),
};

UENUM(BlueprintType)
enum class EDoorSpeed : uint8
{
	Slow	UMETA(DisplayName = "Slow"),
	Fast	UMETA(DisplayName = "Fast"),
};

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABMapDoorInteraction : public ABMapBaseInteraction
{
	GENERATED_BODY()
public:
	ABMapDoorInteraction();
	virtual EInteraction GetInteractionType() const
	{
		return EInteraction::OpenDoor;
	}

	UFUNCTION(BlueprintCallable)
	bool IsBack() const
	{
		return bBack;
	}
	UFUNCTION(BlueprintCallable)
	bool IsFront() const
	{
		return bFront;
	}
	UFUNCTION(BlueprintCallable)
	bool IsInOutValue() const
	{
		return bInValue;
	}

	virtual void MapObjOpen() override;

	virtual void MapObjFastOpen() override;

	UFUNCTION(BlueprintCallable)
	EDoorSpeed GetDoorSpeed() const
	{
		return DoorSpeed;
	}

	UFUNCTION(BlueprintCallable)
	EDoorState GetDoorState() const
	{
		return DoorState;
	}

	UFUNCTION(BlueprintCallable)
	void SetDoorState(EDoorState _State)
	{
		DoorState = _State;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	EDoorState DoorState = EDoorState::None;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	EDoorSpeed DoorSpeed = EDoorSpeed::Slow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	uint8 bInValue : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	uint8 bFront : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	uint8 bBack : 1;


};
