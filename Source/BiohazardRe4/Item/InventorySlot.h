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
	inline void SetItem(class ABInventoryItem* _Item)
	{
		Item = _Item;
	}
	inline void ClearItem()
	{
		Item = nullptr;
	}
	inline ABInventoryItem* GetItem()
	{
		return Item;
	}
	inline void SetPosition(const FIntPoint& _Position)
	{
		Position = _Position;
	}
	inline FIntPoint GetPosition() const
	{
		return Position;
	}
	// �ش� ĭ�� �������� �ִ°� ���θ� ��ȯ
	inline bool HasItem() const
	{
		return Item != nullptr;
	}
	inline void SetSubSlot()
	{
		bIsSubSlot = true;
	}
	inline bool IsSubSlot() const
	{
		return bIsSubSlot;
	}
private:
	class ABInventoryItem* Item;	// ������ ������
	
	UPROPERTY()
	FIntPoint Position;

	UPROPERTY()
	uint32 bIsSubSlot : 1 = false;
};
