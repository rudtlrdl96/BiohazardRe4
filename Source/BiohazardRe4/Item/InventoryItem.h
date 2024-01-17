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

	// 아이템 데이터를 지정한다
	UFUNCTION(BlueprintCallable)
	void SetItemData(const FBItemData& _Data);

	uint8 bIsMove : 1;
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Location;	// 아이템의 위치 (인벤토리상 위치)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count;		// 아이템의 개수

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	FBItemData Data;	// 아이템 데이터
};
