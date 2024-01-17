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
	UBInventoryItem* GetItem()
	{
		return Item;
	}
	void SetLocation(const FIntPoint& _Location)
	{
		Location = _Location;
	}
	FIntPoint GetLocation()
	{
		return Location;
	}

	// �ش� ĭ�� �������� �ִ°� ���θ� ��ȯ
	bool HasItem()
	{
		return Item != nullptr;
	}

private:
	class UBInventoryItem* Item;	// ������ ������
	
	UPROPERTY()
	FIntPoint Location;

};
