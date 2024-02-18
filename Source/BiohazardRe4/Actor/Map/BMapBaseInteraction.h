// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/Generic/Interface/BInteraction.h"
#include "BMapBaseInteraction.generated.h"

class UBoxComponent;

UCLASS()
class BIOHAZARDRE4_API ABMapBaseInteraction : public AActor , public IBInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABMapBaseInteraction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	bool IsOpen() const
	{
		return bIsOpen;
	}
	UFUNCTION(BlueprintCallable)
	bool IsFastOpen() const
	{
		return bIsFastOpen;
	}
	UFUNCTION(BlueprintCallable)
	bool IsClose() const
	{
		return bIsClose;
	}
	UFUNCTION(BlueprintCallable)
	void IsSetOpen(uint8 Value) 
	{
		bIsOpen = Value;
	}

	UFUNCTION(BlueprintCallable)
	void IsSetFastOpen(uint8 Value) 
	{
		bIsFastOpen=Value;
	}
	UFUNCTION(BlueprintCallable)
	void IsSetCloseValue(uint8 Value)
	{
		bIsClose = Value;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Open")
	UBoxComponent* ATrigger = nullptr;
	virtual bool AbleInteraction() const override;
	virtual EInteraction GetInteractionType() const override;

	UFUNCTION(BlueprintCallable)
	virtual void MapObjOpen(const FVector& _Location);

	UFUNCTION(BlueprintCallable)
	virtual void MapObjClose(const FVector& _Location);

	UFUNCTION(BlueprintCallable)
	virtual void MapObjFastOpen(const FVector& _Location);
protected:
	uint8 bIsOpen : 1 = false;

	uint8 bIsClose : 1 = false;
	uint8 bIsFastOpen : 1 = false;
	UPROPERTY(BlueprintReadWrite, Category = "InterValue")
	uint8 bInter : 1;


};
