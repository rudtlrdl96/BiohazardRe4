// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InventoryManager.generated.h"

struct InventorySlot
{

	uint8 bHasItem : 1;
	class UBInventoryItem* Item;

	InventorySlot()
		: bHasItem(0), Item(nullptr)
	{

	}

	inline void SetItem(UBInventoryItem* _Item)
	{
		Item = _Item;
		bHasItem = 1;
	}

	inline bool IsEmpty()
	{
		return !bHasItem;
	}


};

UCLASS(ClassGroup = "Inventory", meta = (BlueprintSpawnableComponent))
class BIOHAZARDRE4_API UBInventoryManager : public USceneComponent
{
	GENERATED_BODY()

public:
	UBInventoryManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	const FIntPoint CaseSize = { 10, 7 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GridScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector GridStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;

	UPROPERTY(VisibleAnywhere)
	TArray<UBInventoryItem*> Items;

	TArray<TArray<InventorySlot>> Slot;

	void AddItem(const FName& _Name);

	FIntPoint FindEmptySlot(const FIntPoint& _Scale);

	void PlaceItemSlot(UBInventoryItem* _Item, const FIntPoint& _Location);
};
