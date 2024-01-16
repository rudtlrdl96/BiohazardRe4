// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "InventoryManager.generated.h"

struct InventorySlot
{
	uint8 bHasItem : 1;
	UInventoryItem* Item;
};

UCLASS()
class BIOHAZARDRE4_API UInventoryManager : public UObject
{
	GENERATED_BODY()

public:
	UInventoryManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;
};
