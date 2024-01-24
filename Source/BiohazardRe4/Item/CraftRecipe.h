// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "CraftRecipe.generated.h"
/**
 *  아이템 조합
 */
USTRUCT(BlueprintType)
struct FBCraftRecipe : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FBCraftRecipe()
		: ResultItem(EItemCode::Empty), AItem(EItemCode::Empty), BItem(EItemCode::Empty)
	{

	}

	// A 아이템 + B 아이템 을 조합하여 Result를 생성한다

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
