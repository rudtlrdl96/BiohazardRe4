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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;	// ������ ���̺� (������ �����ͺ��̽�)

	UPROPERTY(VisibleAnywhere)
	TArray<class UBInventoryItem*> Items;		// ���� ���� ��� �����۵��� �迭

	TArray<class UBInventorySlot*> MainSlot;	// 2���� �κ��丮 ����

	TArray<class UBInventorySlot*> SubSlot;	// 2���� ���� �κ��丮 ���� (������ ����)

// Function

	// �������� �߰��Ѵ�
	void AddItem(const FName& Name);
	// �������� �߰��Ѵ�
	void AddItem(EItemCode ItemCode);
	// Ư�� ũ���� �������� �� ������ �����Ѵٸ� true�� ��ȯ�Ѵ�
	bool IsEmptySlot(const FIntPoint& Scale);
	// �Էµ� ũ��� ��ġ�� �������� ���� �� �ִٸ� true�� ��ȯ�Ѵ�
	bool IsEmptySlot(const FIntPoint& Pos, const FIntPoint& Scale);
	// �������� �ش� ���Կ� ���� �� �ִٸ� true�� ��ȯ�Ѵ�
	bool IsEmptySlot(const UBInventorySlot* Slot, const UBInventoryItem* Item);
	// �������� ���ø���
	void RaiseItem(UBInventoryItem* Item);
	// �������� �̵�
	void MoveItem(UBInventoryItem* Item, const UBInventorySlot* Slot);
	// ������ �̵��� Ȯ�����´�
	void MoveItemConfirm(UBInventoryItem* Item, const UBInventorySlot* Slot);
	// �������� ��ȯ�� �� �ִ��� ���θ� �˾Ƴ���
	bool CheckChange(UBInventoryItem* Item, const UBInventorySlot* Slot);
	// ���� ����ִ� �������� ���� ���� �� �ڸ��� �ִ� �������� ���ø���
	UBInventoryItem* ChangeItem(UBInventoryItem* Item, const UBInventorySlot* Slot);
	// Ư�� ũ���� �������� �� ������ �ִٸ� �� ��ġ�� ��ȯ�Ѵ�. �������� �� ������ ���ٸ� FIntPoint::NoneValue�� ����
	FIntPoint FindEmptySlot(const FIntPoint& Scale);
	// �ش� ��ġ�� �������� �����͸� �޴´�
	UBInventoryItem* FindItem(const UBInventorySlot* Slot);
	// �ش� ��ġ�� �������� �����͸� �޴´�
	UBInventoryItem* FindItem(const FIntPoint& Pos, bool IsSubSlot = false);
	// �ش� �������� �������� ã�Ƽ� �����Ѵ�
	UBInventoryItem* FindItemRange(const FIntPoint& Pos, const FIntPoint& Size, bool IsSubSlot = false);
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

private:
	// �������� �߰��Ѵ�
	void CreateItem(const FBItemData& Data);
	// �������� ��ġ�Ѵ�
	void PlaceItemSlot(UBInventoryItem* Item, const FIntPoint& Pos);
	// �������� ��ġ�Ѵ�
	void PlaceItemSlot(UBInventoryItem* Item, const UBInventorySlot* Slot);
	// �������� ���� ��ġ�� �ִ� ������ �����Ѵ�
	void ClearSlot(const FIntPoint& Location, const FIntPoint& Size, bool IsSubSlot = false);

};
