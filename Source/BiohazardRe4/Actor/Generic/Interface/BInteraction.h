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
enum class EInteraction : uint8
{
	None			UMETA(DisplayName = "None"),
	AttackMonster	UMETA(DisplayName = "AttackMonster"),
	GroggyMonster	UMETA(DisplayName = "GroggyMonster"),
	JumpObstacle	UMETA(DisplayName = "JumpObstacle"),
	FallCliff		UMETA(DisplayName = "FallCliff"),
	OpenDoor		UMETA(DisplayName = "OpenDoor"),
	DropItem		UMETA(DisplayName = "DropItem"),
	WalkPlayer		UMETA(DisplayName = "WalkPlayer"),
	JogPlayer		UMETA(DisplayName = "JogPlayer"),
	StoreEnter		UMETA(DisplayName = "StoreEnter"),
};

/**
 * 
 */
class BIOHAZARDRE4_API IBInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// ���� ��ȣ�ۿ��� ������ ��������
	virtual bool AbleInteraction() const = 0;

	// ���� ��ȣ�ۿ��� ���¸� ��ȯ
	virtual EInteraction GetInteractionType() const = 0;

	// UI Pivot ��ġ�� ��ȯ
	virtual FVector GetUIPivot() const
	{
		return FVector::ZeroVector;
	}

	// ��ȣ�ۿ� UI�� ǥ���Ѵ�
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void EnableInteractionUI();
};