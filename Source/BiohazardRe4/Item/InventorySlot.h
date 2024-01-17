// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "InventorySlot.generated.h"


// 인벤토리내 1칸에 대한 정보를 표현하는 클래스
UCLASS()
class BIOHAZARDRE4_API UBInventorySlot : public UBoxComponent
{
	GENERATED_BODY()

public:

	UBInventorySlot();

	// 해당 칸에 아이템을 지정한다. 아이템이 사라지기 전까지 HasItem이 true로 반환
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

	// 해당 칸에 아이템이 있는가 여부를 반환
	bool HasItem()
	{
		return Item != nullptr;
	}

private:
	class UBInventoryItem* Item;	// 아이템 포인터
	
	UPROPERTY()
	FIntPoint Location;

};
