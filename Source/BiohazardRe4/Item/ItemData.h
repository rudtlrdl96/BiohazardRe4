// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

/*
* 아이템 정보 구조체
*/

USTRUCT(BlueprintType)
struct FBItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FBItemData()
		: ItemName(""), ItemSize(1, 1)
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemInformation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint ItemSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int32 MaxCount;	// 최대 몇개까지 보유할 수 있는지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FVector Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FVector TurnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FRotator TurnRotation;
};
