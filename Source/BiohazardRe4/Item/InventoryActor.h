// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "InventoryActor.generated.h"

#define TO_KEY(EnumValue) static_cast<int32>(EnumValue)

UCLASS()
class BIOHAZARDRE4_API ABInventoryActor : public AActor
{
	GENERATED_BODY()

	enum class EInventoryState
	{
		Default = 0,
		Drag = 1,
	};

public:	
	// Sets default values for this actor's properties
	ABInventoryActor();

	// 아이템을 추가한다
	UFUNCTION(BlueprintCallable)
	void AddItem(const FName& _Name);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UBInventoryManager* Inventory;						// 인벤토리 관리 컴포넌트

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UBInventoryWidget> InventoryWidgetClass;	// 인벤토리 위젯 클래스

	UPROPERTY()
	UBInventoryWidget* Widget;									// 인벤토리 위젯

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* BackgroundMesh;	// 배경 매쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class USkeletalMeshComponent* CaseMesh;	// 가방 메쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UCameraComponent* Camera;			// 카메라

	// __________________Input

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* DefaultMappingContext = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* SelectAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* DragAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* DebugAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UEnhancedInputLocalPlayerSubsystem* Subsystem;	// EnhancedSubSystem

	// _________Inventory__________

	UPROPERTY()
	class UBFsm* FSMComp;

	UPROPERTY()
	class UBInventorySlot* SelectSlot;		// 현재 선택한 슬롯
	class UBInventoryItem* SelectItem;		// 현재 선택한 슬롯
	uint8 bIsDragMove : 1;

private:

	void DebugAddPistol()
	{
		AddItem("PistolAmmo");
	}

	void Click();
	void DragStart();
	void DragTrigger();
	void DragCancel();

	void DefaultEnter();
	void DefaultUpdate(float _DeltaTime);
	void DefaultExit();

	void DragEnter();
	void DragUpdate(float _DeltaTime);
	void DragExit();
};
