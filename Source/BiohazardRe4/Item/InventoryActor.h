// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "InventoryActor.generated.h"

UCLASS()
class BIOHAZARDRE4_API ABInventoryActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABInventoryActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 아이템을 추가한다
	UFUNCTION(BlueprintCallable)
	void AddItem(const FName& _Name);
	void DebugAddPistol()
	{
		AddItem("PistolAmmo");
	}

	void Click();

	UPROPERTY(VisibleAnywhere)
	class UBInventoryManager* Inventory;
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UBInventoryWidget> InventoryWidgetClass;
	UPROPERTY()
	UBInventoryWidget* Widget;

	class UBInventorySlot* SelectSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* BackgroundMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class USkeletalMeshComponent* CaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UCameraComponent* Camera;

	// Input
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* DefaultMappingContext = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* ClickAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* DebugAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UEnhancedInputLocalPlayerSubsystem* Subsystem;
};
