// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InventoryManager.generated.h"

#define CaseLocation(X, Y) FVector(X * GridScale, Y * GridScale, 0) + GridStart
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

	const FIntPoint CaseSize = { 10, 7 };	// �κ��丮�� ũ��

	const float GridScale = 5.0f;	// �׸��� ��ĭ�� ũ��
	const FVector GridStart = { -20.0, -12.5, -1 };	// ũ������ ��������

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;	// ������ ���̺� (������ �����ͺ��̽�)

	UPROPERTY(VisibleAnywhere)
	TArray<class UBInventoryItem*> Items;		// ���� ���� ��� �����۵��� �迭

	TArray<class UBInventorySlot*> Slot;	// 2���� �κ��丮 ����

// Function

public:
	// �������� �߰��Ѵ�
	void AddItem(const FName& _Name);

	// Ư�� ũ���� �������� �� ������ �����Ѵٸ� true�� ��ȯ�Ѵ�
	bool IsEmptySlot(const FIntPoint& _Scale);
	// �Էµ� ũ��� ��ġ�� �������� ���� �� �ִٸ� true�� ��ȯ�Ѵ�
	bool IsEmptySlot(const FIntPoint& _Pos, const FIntPoint& _Scale);
	// �������� �ش� ���Կ� ���� �� �ִٸ� true�� ��ȯ�Ѵ�
	bool IsEmptySlot(const FIntPoint& _Pos, const UBInventoryItem* _Item);
	
	void MoveItem(UBInventoryItem* _Item, const FIntPoint& _Pos);
	void MoveItemConfirm(UBInventoryItem* _Item, const FIntPoint& _Pos);

private:

	// Ư�� ũ���� �������� �� ������ �ִٸ� �� ��ġ�� ��ȯ�Ѵ�. �������� �� ������ ���ٸ� FIntPoint::NoneValue�� ����
	FIntPoint FindEmptySlot(const FIntPoint& _Scale);

	// �������� ��ġ�Ѵ�
	void PlaceItemSlot(UBInventoryItem* _Item, const FIntPoint& _Pos);

	// �������� ���� ��ġ�� �ִ� ������ �����Ѵ�
	void ClearSlot(const FIntPoint& Location, const FIntPoint& Size);
};
