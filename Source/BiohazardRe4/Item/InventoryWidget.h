// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"
/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetItemData(const FBItemData& Data);
	inline void ClearItemData();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	FString ItemName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	FString ItemInformation;
};
