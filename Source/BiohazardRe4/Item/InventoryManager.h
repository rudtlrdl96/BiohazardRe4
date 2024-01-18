// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InventoryManager.generated.h"

static const FIntPoint CaseSize = { 10, 7 };			// �κ��丮�� ũ��
static const float GridScale = 5.0f;					// �׸��� ��ĭ�� ũ��
static const FVector GridStart = { -22.5, -12.5, -1 };	// ũ������ ��������

#define CaseLocation(Position) FVector(Position.X * GridScale, Position.Y * GridScale, 0) + GridStart

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

	TArray<class UBInventorySlot*> Slot;	// 2���� �κ��丮 ����

// Function

	// �������� �߰��Ѵ�
	void AddItem(const FName& _Name);
	// Ư�� ũ���� �������� �� ������ �����Ѵٸ� true�� ��ȯ�Ѵ�
	bool IsEmptySlot(const FIntPoint& _Scale);
	// �Էµ� ũ��� ��ġ�� �������� ���� �� �ִٸ� true�� ��ȯ�Ѵ�
	bool IsEmptySlot(const FIntPoint& _Pos, const FIntPoint& _Scale);
	// �������� �ش� ���Կ� ���� �� �ִٸ� true�� ��ȯ�Ѵ�
	bool IsEmptySlot(const FIntPoint& _Pos, const UBInventoryItem* _Item);
	// �������� ���ø���
	void RaiseItem(UBInventoryItem* _Item);
	// �������� �̵�
	void MoveItem(UBInventoryItem* _Item, const FIntPoint& _Pos);
	// ������ �̵��� Ȯ�����´�
	void MoveItemConfirm(UBInventoryItem* _Item, const FIntPoint& _Pos);
	// �������� ��ȯ�� �� �ִ��� ���θ� �˾Ƴ���
	bool CheckChange(UBInventoryItem* _Item, const FIntPoint& _Pos);
	UBInventoryItem* ChangeItem(UBInventoryItem* Item, const FIntPoint& Pos);
	// Ư�� ũ���� �������� �� ������ �ִٸ� �� ��ġ�� ��ȯ�Ѵ�. �������� �� ������ ���ٸ� FIntPoint::NoneValue�� ����
	FIntPoint FindEmptySlot(const FIntPoint& _Scale);
	// �ش� ��ġ�� �������� �����͸� �޴´�
	UBInventoryItem* FindItem(const FIntPoint& Pos);
	// �ش� ������ �������� Items�� �޾ƿ´�
	void FindItemRange(const FIntPoint& Pos, const FIntPoint& Size, TArray<UBInventoryItem*>& FindItems);
	// �ش� �������� �������� ã�Ƽ� �����Ѵ�
	UBInventoryItem* FindItemRange(const FIntPoint& Pos, const FIntPoint& Size);
	// �ش� ��ġ�� ��ȿ�� �������� �˻��մϴ�
	bool IsVaildSlot(const FIntPoint& Pos);
	// �ش� ��ġ�� ��ȿ�� �������� �˻��մϴ�
	bool IsVaildRange(const FIntPoint& Pos, const FIntPoint& Size);
	// �ش� ���Կ� �������� �ְų� ���� ���̸� false�� �����Ѵ� (��ȿ�� ������ �� true)
	bool CheckSlot(const FIntPoint& Pos);
	// �ش� ���Կ� �������� �ְų� ���� ���̸� false�� �����Ѵ�, IgnoreItem�� ��ģ ��쿡�� �����Ѵ�
	bool CheckSlot(const FIntPoint& Pos, const UBInventoryItem* IgnoreItem);
	// �ش� ������ �������� �ְų� ���� ���̸� false�� �����Ѵ� (��ȿ�� ������ �� true)
	bool CheckSlotRange(const FIntPoint& Pos, const FIntPoint& Size);

private:
	// �������� ��ġ�Ѵ�
	void PlaceItemSlot(UBInventoryItem* _Item, const FIntPoint& _Pos);
	// �������� ���� ��ġ�� �ִ� ������ �����Ѵ�
	void ClearSlot(const FIntPoint& Location, const FIntPoint& Size);

};
