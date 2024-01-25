// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
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
		Drag,
		Select,
		Craft,
		Drop,
		CloseCheck,
	};

public:	
	// Sets default values for this actor's properties
	ABInventoryActor();

	static ABInventoryActor* Instance;
	// �������� �߰��Ѵ�
	
	void AddItem(EItemCode ItemCode);

	void AddItem(const FName& _Name);

	UFUNCTION(BlueprintCallable)
	void OpenInventory();

	UFUNCTION(BlueprintCallable)
	void OpenSub();

	UFUNCTION(BlueprintCallable)
	void CloseSub();

	UFUNCTION()
	void OpenCraft();

	UFUNCTION()
	void CompleteCraft();

	UFUNCTION()
	void ItemUse();

	UFUNCTION()
	void DropItem();

	UFUNCTION()
	void CompleteDrop();

	UFUNCTION()
	void DropCancel();

	UFUNCTION()
	void CloseCancel();

	UFUNCTION()
	void CloseInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UBInventoryManager* Inventory;						// �κ��丮 ���� ������Ʈ

	// ___________________________ Widget ___________________________________

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UBInventoryWidgetMain> InventoryWidgetClass;	// �κ��丮 ���� Ŭ����

	UPROPERTY()
	UBInventoryWidgetMain* Widget;									// �κ��丮 ����

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UBInventoryWidgetBehavior> BehaviorWidgetClass;	// ������ �ൿ ���� Ŭ����

	UPROPERTY()
	UBInventoryWidgetBehavior* BehaviorWidget;							// ������ �ൿ ����

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UBInventoryWidgetCraft> CraftWidgetClass;	// ������ �ൿ ���� Ŭ����

	UPROPERTY()
	UBInventoryWidgetCraft* CraftWidget;							// ������ �ൿ ����

	// ___________________________ Component ___________________________________

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* BackgroundMesh;	// ��� �Ž�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class USkeletalMeshComponent* CaseMesh;	// ���� �޽�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* SubCaseMesh;	// ������ ���� �޽�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UCameraComponent* Camera;			// ī�޶�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UBInventoryCursor* Cursor;			// Ŀ��
	
	// _________________Timeline
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CurveFloat;
	FTimeline Timeline;

	// _________________Animation

	UPROPERTY()
	UAnimSequence* OpenAnim;

	// __________________Input

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* DefaultMappingContext = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* SelectAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* DragAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* TurnAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* CancelAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UEnhancedInputLocalPlayerSubsystem* Subsystem;	// EnhancedSubSystem

	// _________Inventory__________

	UPROPERTY()
	class UBFsm* FSMComp;

	UPROPERTY()
	class UBInventorySlot* SelectSlot;		// ���� ������ ����
	class ABInventoryItem* SelectItem;		// ���� ������ ����
	uint8 bIsDragMove : 1;

	UFUNCTION(BlueprintCallable)
	void DebugAdd(EItemCode Code)
	{
		AddItem(Code);
	}

private:


	void Click();
	void Cancel();
	void DragStart();
	void DragTrigger();
	void DragCancel();
	void Turn();

	void DefaultEnter();
	void DefaultUpdate(float _DeltaTime);
	void DefaultExit();

	void DragEnter();
	void DragUpdate(float _DeltaTime);
	void DragExit();

};
