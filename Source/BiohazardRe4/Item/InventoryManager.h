// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InventoryManager.generated.h"

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

private:

	// Ư�� ũ���� �������� �� ������ �ִٸ� �� ��ġ�� ��ȯ�Ѵ�. �������� �� ������ ���ٸ� FIntPoint::NoneValue�� ����
	FIntPoint FindEmptySlot(const FIntPoint& _Scale);

	// �������� ��ġ�Ѵ�
	void PlaceItemSlot(UBInventoryItem* _Item, const FIntPoint& _Location);
};
