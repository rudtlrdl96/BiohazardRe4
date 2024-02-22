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
		Wait = -1,		// 인벤토리를 열지 않은 상태
		Default = 0,	// 인벤토리를 연 기본 상태
		Drag,			// 아이템을 드래그하여 이동시키는 상태
		Select,			// 아이템을 선택하여 아이템 행동창이 열린 상태
		Craft,			// 아이템을 조합하는 상태
		Drop,			// 아이템을 버리는 것을 확인하는 상태
		CloseCheck,		// 아이템을 버리고 인벤토리를 닫는 것을 확인하는 상태
		Investigate,	// 아이템을 조사하는 상태
		QuickSlot,		// 퀵슬롯을 지정하는 상태
	};

public:	
	// Sets default values for this actor's properties
	ABInventoryActor();

	static ABInventoryActor* Instance;
	
	// 아이템을 추가한다
	UFUNCTION(BlueprintCallable)
	void AddItem(EItemCode ItemCode, int Num = 1);

	// 아이템을 추가한다
	UFUNCTION(BlueprintCallable)
	void AddItemRowName(FName ItemRowName, int Num = 1);

	// 돈을 획득한다
	UFUNCTION(BlueprintCallable)
	void AddMoney(int Num);

	// 해당 코드의 아이템의 개수를 찾는다
	UFUNCTION(BlueprintCallable)
	int GetItemCount(EItemCode ItemCode) const;

	// 해당 코드의 아이템을 Num개 만큼 제거한다
	UFUNCTION(BlueprintCallable)
	void RemoveItem(EItemCode ItemCode, int Num = 1);

	void RemoveQuickSlot(EItemCode Code);
	void RemoveQuickSlot(class ABInventoryItem* Item);

	// 해당 퀵슬롯의 무기를 찾는다
	UFUNCTION(BlueprintCallable)
	ABInventoryWeapon* GetQuickSlot(int SlotNum);

	UFUNCTION(BlueprintCallable)
	ABInventoryWeapon* FindWeapon(EItemCode ItemCode);

	// 아이템이 들어갈 공간이 존재한다면 true를 반환한다
	UFUNCTION(BlueprintCallable)
	bool IsEmptySlot(EItemCode ItemCode, int Count);

	// * 인벤토리를 연다, UI를 킬때 이걸 실행해주세요
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
	class UBInventoryManager* Inventory;						// 인벤토리 관리 컴포넌트

	TArray<class ABInventoryWeapon*> QuickSlot;	// 퀵슬롯

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABInventoryItem> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABInventoryWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABInventoryWeapon> KnifeClass;

	// ___________________________ Widget ___________________________________

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UBInventoryWidgetMain> InventoryWidgetClass;	// 인벤토리 위젯 클래스

	UPROPERTY()
	UBInventoryWidgetMain* Widget;									// 인벤토리 위젯

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UBInventoryWidgetBehavior> BehaviorWidgetClass;	// 아이템 행동 위젯 클래스

	UPROPERTY()
	UBInventoryWidgetBehavior* BehaviorWidget;							// 아이템 행동 위젯

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UBInventoryWidgetCraft> CraftWidgetClass;	// 아이템 행동 위젯 클래스

	UPROPERTY()
	UBInventoryWidgetCraft* CraftWidget;							// 아이템 행동 위젯

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UBInventoryWidgetQuickSlot> QuickSlotWidgetClass;	// 아이템 행동 위젯 클래스

	UPROPERTY()
	UBInventoryWidgetQuickSlot* QuickSlotWidget;							// 아이템 행동 위젯

	// ___________________________ Component ___________________________________

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* BackgroundMesh;	// 배경 매쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class USkeletalMeshComponent* CaseMesh;	// 가방 메쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* SubCaseMesh;	// 버리는 가방 메쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UCameraComponent* Camera;			// 카메라

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UBInventoryCursor* Cursor;			// 커서
	
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
	class UBInventorySlot* SelectSlot;		// 현재 선택한 슬롯
	class ABInventoryItem* SelectItem;		// 현재 선택한 슬롯
	uint8 bIsDragMove : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Money;

	UFUNCTION(BlueprintCallable)
	void DebugAdd(EItemCode Code, int Num)
	{
		AddItem(Code, Num);
	}

private:

	// 아이템을 추가한다
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
