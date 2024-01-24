// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ItemData.h"
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

	// �������� �߰��Ѵ�
	void AddItem(const FName& Name);
	// �������� �߰��Ѵ�
	void AddItem(EItemCode ItemCode);
	// �������� �����Ѵ�. Num�� ��ŭ �����մϴ�
	void RemoveItem(EItemCode ItemCode, int Num = 1);
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
	int GetItemNum(EItemCode Code);

private:
	// �������� �߰��Ѵ�
	void CreateItem(const FBItemData& Data);
	// �������� ��ġ�Ѵ�
	void PlaceItemSlot(ABInventoryItem* Item, const FIntPoint& Pos);
	// �������� ��ġ�Ѵ�
	void PlaceItemSlot(ABInventoryItem* Item, const UBInventorySlot* Slot);
	// �������� ���� ��ġ�� �ִ� ������ �����Ѵ�
	void ClearSlot(const FIntPoint& Location, const FIntPoint& Size, bool IsSubSlot = false);

};
