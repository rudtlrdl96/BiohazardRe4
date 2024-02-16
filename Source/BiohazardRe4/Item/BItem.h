// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/Generic/Interface/BInteraction.h"
#include "ItemData.h"
#include "BItem.generated.h"

UCLASS()
class BIOHAZARDRE4_API ABItem : public AActor, public IBInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 현재 상호작용이 가능한 상태인지
	virtual bool AbleInteraction() const { return !bIsPickup; }
	// 현재 상호작용의 상태를 반환
	virtual EInteraction GetInteractionType() const { return EInteraction::DropItem; }
	// UI Pivot 위치를 반환
	virtual FVector GetUIPivot() const { return FVector::ZeroVector; }
	// 아이템을 줍는다
	void PickUp();
	UFUNCTION(BlueprintImplementableEvent)
	void PickUpEvent();

	UPROPERTY(EditAnywhere ,BlueprintReadWrite, Category="Item")
	EItemCode ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Count;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
	uint32 bCanPickup : 1;
	UPROPERTY(BlueprintReadWrite, Category = "Item")
	uint32 bIsPickup : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	class USphereComponent* Collision;
};
