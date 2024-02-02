// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/ItemData.h"
#include "Merchant.generated.h"

UENUM(BlueprintType)
enum class EMerchant : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Open			UMETA(DisplayName = "Open"),
	GiveHand		UMETA(DisplayName = "GiveHand"),
};

UENUM(BlueprintType)
enum class EStoreState : uint8
{
	Title		UMETA(DisplayName = "Title"),
	Buy			UMETA(DisplayName = "Buy"),
	Sell		UMETA(DisplayName = "Sell"),
};

USTRUCT(BlueprintType)
struct FSaleItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemCode ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Price;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Count;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurCount;
};

UCLASS()
class BIOHAZARDRE4_API ABMerchant : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABMerchant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	class ABInventoryActor* GetInventory() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (TitleProperty = "ItemCode"), Category = "Item")
	TArray<FSaleItem> SaleItems;
};
