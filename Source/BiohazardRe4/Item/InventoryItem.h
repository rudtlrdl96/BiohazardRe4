// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ItemData.h"
#include "InventoryItem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIOHAZARDRE4_API UBInventoryItem : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBInventoryItem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetItemData(const FBItemData& _Data);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	FBItemData Data;
};
