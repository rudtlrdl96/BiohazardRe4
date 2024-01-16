// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InventoryManager.generated.h"

// 인벤토리내 1칸에 대한 정보를 표현하는 클래스
class InventorySlot
{

	uint8 bHasItem : 1;				// 해당 칸에 아이템이 있는지 여부
	class UBInventoryItem* Item;	// 아이템 포인터

public:
	InventorySlot()
		: bHasItem(0), Item(nullptr)
	{

	}

	FORCEINLINE UBInventoryItem* GetItem()
	{
		return Item;
	}
	// 해당 칸에 아이템을 지정한다. 아이템이 사라지기 전까지 HasItem이 true로 반환
	FORCEINLINE void SetItem(UBInventoryItem* _Item)
	{
		Item = _Item;
		bHasItem = 1;
	}
	// 해당 칸에 아이템이 있는가 여부를 반환
	FORCEINLINE bool HasItem()
	{
		return bHasItem;
	}


};

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

	const FIntPoint CaseSize = { 10, 7 };	// 인벤토리의 크기

	const float GridScale = 5.0f;	// 그리드 한칸의 크기
	const FVector GridStart = { -20.0, -12.5, -1 };	// 크리드의 시작지점

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;	// 데이터 테이블 (아이템 데이터베이스)

	UPROPERTY(VisibleAnywhere)
	TArray<UBInventoryItem*> Items;		// 현재 지닌 모든 아이템들의 배열

	TArray<TArray<InventorySlot>> Slot;	// 2차원 인벤토리 공간

// Function

public:
	// 아이템을 추가한다
	void AddItem(const FName& _Name);

	// 특정 크기의 아이템이 들어갈 공간이 존재한다면 true를 반환한다
	bool IsEmptySlot(const FIntPoint& _Scale);

private:

	// 특정 크기의 아이템이 들어갈 공간이 있다면 그 위치를 반환한다. 아이템이 들어갈 공간이 없다면 FIntPoint::NoneValue를 리턴
	FIntPoint FindEmptySlot(const FIntPoint& _Scale);

	// 아이템을 배치한다
	void PlaceItemSlot(UBInventoryItem* _Item, const FIntPoint& _Location);
};
