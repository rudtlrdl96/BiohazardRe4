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
	virtual bool AbleInteraction() const override;
	virtual EInteraction GetInteractionType() const override;
protected:

	UFUNCTION(BlueprintCallable)
	virtual void MapObjOpen();

	UFUNCTION(BlueprintCallable)
	virtual void MapObjClose();
	uint8 bIsOpen : 1;

};
