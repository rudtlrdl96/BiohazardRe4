// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CraftRecipe.h"
#include "InventoryManager.generated.h"
static const FIntPoint CaseSize = { 10, 7 };			// �κ��丮�� ũ��
static const FIntPoint SubCaseSize = { 5, 9 };			// �κ��丮�� ũ��
static const float GridScale = 5.0f;					// �׸��� ��ĭ�� ũ��
static const FVector GridStart = { -22.5, -12.5, -1.0 };	// ũ������ ��������
static const FVector SubGridStart = { 35.0, -12.5, 1.0 };	// ũ������ ��������

#define CaseLocation(Position) FVector(Position.X * GridScale, Position.Y * GridScale, 0) + GridStart
#define SubCaseLocation(Position) FVector(Position.X * GridScale, Position.Y * GridScale, 0) + SubGridStart

UCLASS(ClassGroup = "Inventory", meta = (BlueprintSpawnableComponent))
class BIOHAZARDRE4_API UBInventoryManager : public USceneComponent
{
	GENERATED_BODY()

public:
	UBInventoryManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	static UBInventoryManager* Instance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;	// ������ ���̺� (������ �����ͺ��̽�)

	TMultiMap<EItemCode, class ABInventoryItem*> ItemMap;		// ���� ���� ��� �����۵��� �迭

	TArray<class UBInventorySlot*> MainSlot;	// 2���� �κ��丮 ����

	TArray<class UBInventorySlot*> SubSlot;	// 2���� ���� �κ��丮 ���� (������ ����)

// Function
	const FBItemData& FindItemData(EItemCode Code);
	// �������� �߰��Ѵ�
	void AddItem(EItemCode ItemCode, int Num = 1);
	// �������� �ش� ��ġ�� �߰��Ѵ�
	void AddItem(EItemCode ItemCode, const FIntPoint& Pos, int Num = 1);
	// �������� �����Ѵ�. Num�� ��ŭ �����մϴ�
	void RemoveItem(EItemCode ItemCode, int Num = 1);
	// Item�� Num��ŭ �����մϴ�. Item�� ���� �����ϸ� ���� ������ �ٸ� �������� �����մϴ�
	void RemoveItem(ABInventoryItem* Item, int Num = 1);
	// �������� �����Ѵ�
	void CraftItem(const FBCraftRecipe& Recipe);
	// Ư�� ũ���� �������� �� ������ �����Ѵٸ� true�� ��ȯ�Ѵ�
	bool IsEmptySlot(const FIntPoint& Scale);
	// �Էµ� ũ��� ��ġ�� �������� ���� �� �ִٸ� true�� ��ȯ�Ѵ�
	bool IsEmptySlot(const FIntPoint& Pos, const FIntPoint& Scale);
	// �������� �ش� ���Կ� ���� �� �ִٸ� true�� ��ȯ�Ѵ�
	bool IsEmptySlot(const UBInventorySlot* Slot, const ABInventoryItem* Item);
	// �������� ���ø���
	void RaiseItem(ABInventoryItem* Item);
	// �������� �̵�
	void MoveItem(ABInventoryItem* Item, const UBInventorySlot* Slot);
	// ������ �̵��� Ȯ�����´�
	void MoveItemConfirm(ABInventoryItem* Item, const UBInventorySlot* Slot);
	// �������� ��ȯ�� �� �ִ��� ���θ� �˾Ƴ���
	bool CheckChange(ABInventoryItem* Item, const UBInventorySlot* Slot);
	// �ش� �������� �����ϰ� ���� ������ �����Ѵ�
	int ItemMerge(const FBItemData& Data, int Num);
	// ���� ����ִ� �������� ���� ���� �� �ڸ��� �ִ� �������� ���ø���
	ABInventoryItem* ChangeItem(ABInventoryItem* Item, const UBInventorySlot* Slot);
	// Ư�� ũ���� �������� �� ������ �ִٸ� �� ��ġ�� ��ȯ�Ѵ�. �������� �� ������ ���ٸ� FIntPoint::NoneValue�� ����
	FIntPoint FindEmptySlot(const FIntPoint& Scale);
	// �ش� ��ġ�� �������� �����͸� �޴´�
	ABInventoryItem* FindItem(const UBInventorySlot* Slot);
	// �ش� ��ġ�� �������� �����͸� �޴´�
	ABInventoryItem* FindItem(const FIntPoint& Pos, bool IsSubSlot = false);
	// �ش� �������� �������� ã�Ƽ� �����Ѵ�
	ABInventoryItem* FindItemRange(const FIntPoint& Pos, const FIntPoint& Size, bool IsSubSlot = false);
	// �ش� ��ġ�� ��ȿ�� �������� �˻��մϴ�
	bool IsVaildSlot(const FIntPoint& Pos, bool IsSubSlot = false);
	// �ش� ��ġ�� ��ȿ�� �������� �˻��մϴ�
	bool IsVaildSlot(const UBInventorySlot* Slot);
	// �ش� ������ ��ȿ�� �������� �˻��մϴ�
	bool IsVaildSlotRange(const UBInventorySlot* Slot, const FIntPoint& Size);
	// �ش� ���Կ� �������� �ְų� ���� ���̸� false�� �����Ѵ� (��ȿ�� ������ �� true)
	bool CheckSlot(const FIntPoint& Pos);
	// �ش� ���꽽�Կ� �������� �ְų� ���� ���̸� false�� �����Ѵ�, IgnoreItem�� ��ģ ��쿡�� �����Ѵ�
	bool CheckSubSlot(const FIntPoint& Pos);
	// �ش� ������ �������� �ְų� ���� ���̸� false�� �����Ѵ� (��ȿ�� ������ �� true)
	bool CheckSlotRange(const FIntPoint& Pos, const FIntPoint& Size);
	// ���� ���Կ� �������� �ִٸ� true�� �����Ѵ�
	bool HasItemInSubSlot();
	// �ش� �����ۿ� ���� ��ġ���� �޾ƿɴϴ�
	FVector GetItemWorldLocation(ABInventoryItem* Item);
	// �ش� �ڵ��� �������� �κ��丮�� � �ִ��� �޾ƿɴϴ�
	int GetItemCount(EItemCode Code);
	// ���� ���Կ� �ִ� ��� �������� �����ϴ�
	void RemoveAllItemInSubSlot();

private:
	// �������� �߰��Ѵ�
	void CreateItem(const FBItemData& Data, int Num = 1);
	// �������� �ش� ��ġ�� �߰��մϴ�
	void CreateItem(const FBItemData& Data, const FIntPoint& Pos, int Num = 1);
	// �������� ��ġ�Ѵ�
	void PlaceItemSlot(ABInventoryItem* Item, const FIntPoint& Pos);
	// �������� ��ġ�Ѵ�
	void PlaceItemSlot(ABInventoryItem* Item, const UBInventorySlot* Slot);
	// �������� ���� ��ġ�� �ִ� ������ �����Ѵ�
	void ClearSlot(const FIntPoint& Location, const FIntPoint& Size, bool IsSubSlot = false);

};
