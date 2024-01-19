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
		Drag = 1,
	};

public:	
	// Sets default values for this actor's properties
	ABInventoryActor();

	// �������� �߰��Ѵ�
	UFUNCTION(BlueprintCallable)
	void AddItem(const FName& _Name);

	UFUNCTION(BlueprintCallable)
	void OpenInventory();

	UFUNCTION(BlueprintCallable)
	void OpenSub();

	UFUNCTION(BlueprintCallable)
	void CloseSub();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UBInventoryManager* Inventory;						// �κ��丮 ���� ������Ʈ

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UBInventoryWidget> InventoryWidgetClass;	// �κ��丮 ���� Ŭ����

	UPROPERTY()
	UBInventoryWidget* Widget;									// �κ��丮 ����

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UBInventoryBehavior> BehaviorWidgetClass;	// ������ �ൿ ���� Ŭ����

	UPROPERTY()
	UBInventoryBehavior* BehaviorWidget;							// ������ �ൿ ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* BackgroundMesh;	// ��� �Ž�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class USkeletalMeshComponent* CaseMesh;	// ���� �޽�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
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
	class UInputAction* DebugAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UEnhancedInputLocalPlayerSubsystem* Subsystem;	// EnhancedSubSystem

	// _________Inventory__________

	UPROPERTY()
	class UBFsm* FSMComp;

	UPROPERTY()
	class UBInventorySlot* SelectSlot;		// ���� ������ ����
	class UBInventoryItem* SelectItem;		// ���� ������ ����
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
	void Turn();

	void DefaultEnter();
	void DefaultUpdate(float _DeltaTime);
	void DefaultExit();

	void DragEnter();
	void DragUpdate(float _DeltaTime);
	void DragExit();
};
