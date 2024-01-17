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

	const FIntPoint CaseSize = { 10, 7 };	// 인벤토리의 크기

	const float GridScale = 5.0f;	// 그리드 한칸의 크기
	const FVector GridStart = { -20.0, -12.5, -1 };	// 크리드의 시작지점

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;	// 데이터 테이블 (아이템 데이터베이스)

	UPROPERTY(VisibleAnywhere)
	TArray<class UBInventoryItem*> Items;		// 현재 지닌 모든 아이템들의 배열

	TArray<class UBInventorySlot*> Slot;	// 2차원 인벤토리 공간

// Function

public:
	// 아이템을 추가한다
	void AddItem(const FName& _Name);

	// 특정 크기의 아이템이 들어갈 공간이 존재한다면 true를 반환한다
	bool IsEmptySlot(const FIntPoint& _Scale);
	// 입력된 크기와 위치에 아이템을 놓을 수 있다면 true를 반환한다
	bool IsEmptySlot(const FIntPoint& _Location, const FIntPoint& _Scale);
	// 아이템이 해당 슬롯에 놓을 수 있다면 true를 반환한다
	bool IsEmptySlot(const FIntPoint& _Location, const UBInventoryItem* _Item);
	
	void MoveItem(UBInventoryItem* _Item, const FIntPoint& _Location);
	void MoveItemConfirm(UBInventoryItem* _Item, const FIntPoint& _Location);

private:

	// 특정 크기의 아이템이 들어갈 공간이 있다면 그 위치를 반환한다. 아이템이 들어갈 공간이 없다면 FIntPoint::NoneValue를 리턴
	FIntPoint FindEmptySlot(const FIntPoint& _Scale);

	// 아이템을 배치한다
	void PlaceItemSlot(UBInventoryItem* _Item, const FIntPoint& _Location);

	// 아이템의 기존 위치에 있는 슬롯을 정리한다
	void ClearSlot(const FIntPoint& Location, const FIntPoint& Size);
};
