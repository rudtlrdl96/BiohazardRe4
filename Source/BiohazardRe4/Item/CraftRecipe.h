// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "CraftRecipe.generated.h"
/**
 *  ������ ����
 */
USTRUCT(BlueprintType)
struct FBCraftRecipe : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FBCraftRecipe()
		: ResultItem(EItemCode::Empty), AItem(EItemCode::Empty), BItem(EItemCode::Empty)
	{

	}

	// A ������ + B ������ �� �����Ͽ� Result�� �����Ѵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ResultName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCode ResultItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ResultNum = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCode AItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ANum = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCode BItem = EItemCode::Empty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BNum = 1;

};
