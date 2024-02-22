// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "InventoryActor.generated.h"

#define TO_KEY(EnumValue) static_cast<int32>(EnumValue)

struct FInputActionInstance;
UCLASS()
class BIOHAZARDRE4_API ABInventoryActor : public AActor
{
	GENERATED_BODY()

	enum class EInventoryState
	{
		Wait = -1,		// �κ��丮�� ���� ���� ����
		Default = 0,	// �κ��丮�� �� �⺻ ����
		Drag,			// �������� �巡���Ͽ� �̵���Ű�� ����
		Select,			// �������� �����Ͽ� ������ �ൿâ�� ���� ����
		Craft,			// �������� �����ϴ� ����
		Drop,			// �������� ������ ���� Ȯ���ϴ� ����
		CloseCheck,		// �������� ������ �κ��丮�� �ݴ� ���� Ȯ���ϴ� ����
		Investigate,	// �������� �����ϴ� ����
		QuickSlot,		// �������� �����ϴ� ����
	};

public:	
	// Sets default values for this actor's properties
	ABInventoryActor();

	static ABInventoryActor* Instance;
	
	// �������� �߰��Ѵ�
	UFUNCTION(BlueprintCallable)
	void AddItem(EItemCode ItemCode, int Num = 1);

	// �������� �߰��Ѵ�
	UFUNCTION(BlueprintCallable)
	void AddItemRowName(FName ItemRowName, int Num = 1);

	// ���� ȹ���Ѵ�
	UFUNCTION(BlueprintCallable)
	void AddMoney(int Num);

	// �ش� �ڵ��� �������� ������ ã�´�
	UFUNCTION(BlueprintCallable)
	int GetItemCount(EItemCode ItemCode) const;

	// �ش� �ڵ��� �������� Num�� ��ŭ �����Ѵ�
	UFUNCTION(BlueprintCallable)
	void RemoveItem(EItemCode ItemCode, int Num = 1);

	void RemoveQuickSlot(EItemCode Code);
	void RemoveQuickSlot(class ABInventoryItem* Item);

	// �ش� �������� ���⸦ ã�´�
	UFUNCTION(BlueprintCallable)
	ABInventoryWeapon* GetQuickSlot(int SlotNum);

	UFUNCTION(BlueprintCallable)
	ABInventoryWeapon* FindWeapon(EItemCode ItemCode);

	// �������� �� ������ �����Ѵٸ� true�� ��ȯ�Ѵ�
	UFUNCTION(BlueprintCallable)
	bool IsEmptySlot(EItemCode ItemCode, int Count);

	// * �κ��丮�� ����, UI�� ų�� �̰� �������ּ���
	UFUNCTION(BlueprintCallable)
	void OpenInventory();

	UFUNCTION(BlueprintCallable)
	void OpenSub();
	UFUNCTION()
	void OpenCraft();
	UFUNCTION()
	void CompleteCraft();
	UFUNCTION(BlueprintCallable)
	void CloseSub();
	UFUNCTION()
	void ItemUse();
	UFUNCTION()
	void DropItem();
	UFUNCTION()
	void WeaponEquip();
	UFUNCTION()
	void CompleteDrop();
	UFUNCTION()
	void DropCancel();
	UFUNCTION()
	void CloseCancel();
	UFUNCTION()
	void CloseInventory();
	UFUNCTION()
	void StartInvestigate();
	UFUNCTION()
	void EndInvestigate();
	UFUNCTION()
	void OpenQuickSlot();
	UFUNCTION()
	void CloseQuickSlot();
	UFUNCTION()
	void SetHealPreview();
	UFUNCTION()
	void OffHealPreview();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UBInventoryManager* Inventory;						// �κ��丮 ���� ������Ʈ

	TArray<class ABInventoryWeapon*> QuickSlot;	// ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABInventoryItem> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABInventoryWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABInventoryWeapon> KnifeClass;

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

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UBInventoryWidgetQuickSlot> QuickSlotWidgetClass;	// ������ �ൿ ���� Ŭ����

	UPROPERTY()
	UBInventoryWidgetQuickSlot* QuickSlotWidget;							// ������ �ൿ ����

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	USceneComponent* InvestigatePivot;

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
	class UInputAction* RotateAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UEnhancedInputLocalPlayerSubsystem* Subsystem;	// EnhancedSubSystem

	// _________Sound____________
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_InventoryOpen = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_InventoryClose = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_CursorMove = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_ItemDrag = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_ItemSelect = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_ItemCancel = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_ItemEquip = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_ItemUse = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_ItemCraftOpen = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_ItemCraft = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_ItemDropCheck = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_ItemDrop = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_ItemInvestigate = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_ItemQuickSlot = nullptr;

	// _________Inventory__________

	UPROPERTY()
	class UBFsm* FSMComp;
	UPROPERTY()
	APlayerController* Controller;
	UPROPERTY()
	class ABHUDBase* HUD;

	UPROPERTY()
	class ABLeon* Player;

	UPROPERTY()
	class UBInventorySlot* SelectSlot;		// ���� ������ ����
	class ABInventoryItem* SelectItem;		// ���� ������ ����
	uint8 bIsDragMove : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Money;

	UFUNCTION(BlueprintCallable)
	void DebugAdd(EItemCode Code, int Num)
	{
		AddItem(Code, Num);
	}

private:

	// �������� �߰��Ѵ�
	void StartItem();

	void Click();
	void Cancel();
	void DragStart();
	void DragTrigger();
	void DragCancel();
	void Turn();

	// FSM
	void DefaultEnter();
	void DefaultUpdate(float _DeltaTime);
	void DefaultExit();

	void DragEnter();
	void DragUpdate(float _DeltaTime);
	void DragExit();

	void SelectEnter();
	void SelectExit();

	void InvestigateEnter();
	void InvestigateUpdate(float DeltaTime);
	void InvestigateExit();

	void InvestigateRotate(const FInputActionInstance& _MoveAction);

	void CraftEnter();
	void CraftExit();

	void QuickSlotEnter();
	void QuickSlotExit();

	void DropEnter();
	void DropExit();

	void CloseCheckEnter();
	void CloseCheckExit();

};
