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
	// 해당 칸에 아이템이 있는가 여부를 반환
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
	class ABInventoryItem* Item;	// 아이템 포인터
	
	UPROPERTY()
	FIntPoint Position;

	UPROPERTY()
	uint32 bIsSubSlot : 1 = false;
};
