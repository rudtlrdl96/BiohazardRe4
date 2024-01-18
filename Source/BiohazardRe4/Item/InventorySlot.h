// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "InventorySlot.generated.h"


// �κ��丮�� 1ĭ�� ���� ������ ǥ���ϴ� Ŭ����
UCLASS()
class BIOHAZARDRE4_API UBInventorySlot : public UBoxComponent
{
	GENERATED_BODY()

public:

	UBInventorySlot();

	// �ش� ĭ�� �������� �����Ѵ�. �������� ������� ������ HasItem�� true�� ��ȯ
	void SetItem(class UBInventoryItem* _Item)
	{
		Item = _Item;
	}
	void ClearItem()
	{
		Item = nullptr;
	}
	UBInventoryItem* GetItem()
	{
		return Item;
	}
	void SetPosition(const FIntPoint& _Position)
	{
		Position = _Position;
	}
	FIntPoint GetPosition()
	{
		return Position;
	}

	// �ش� ĭ�� �������� �ִ°� ���θ� ��ȯ
	bool HasItem()
	{
		return Item != nullptr;
	}

private:
	class UBInventoryItem* Item;	// ������ ������
	
	UPROPERTY()
	FIntPoint Position;

};
