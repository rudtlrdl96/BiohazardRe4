// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FBItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FBItemData()
		: Name(""), Scale(1, 1)
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int32 MaxReserves;	// 최대 몇개까지 보유할 수 있는지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	UStaticMesh* Mesh;
};
