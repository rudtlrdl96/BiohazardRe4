// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ItemData.h"
#include "InventoryManager.generated.h"
static const FIntPoint CaseSize = { 10, 7 };			// 인벤토리의 크기
static const FIntPoint SubCaseSize = { 5, 9 };			// 인벤토리의 크기
static const float GridScale = 5.0f;					// 그리드 한칸의 크기
static const FVector GridStart = { -22.5, -12.5, -1.0 };	// 크리드의 시작지점
static const FVector SubGridStart = { 35.0, -12.5, 1.0 };	// 크리드의 시작지점

#define CaseLocation(Position) FVector(Position.X * GridScale, Position.Y * GridScale, 0) + GridStart
#define SubCaseLocation(Position) FVector(Position.X * GridScale, Position.Y * GridScale, 0) + SubGridStart

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
	UDataTable* ItemDataTable;	// 데이터 테이블 (아이템 데이터베이스)

	UPROPERTY(VisibleAnywhere)
	TArray<class UBInventoryItem*> Items;		// 현재 지닌 모든 아이템들의 배열

	TArray<class UBInventorySlot*> MainSlot;	// 2차원 인벤토리 공간

	TArray<class UBInventorySlot*> SubSlot;	// 2차원 서브 인벤토리 공간 (버리는 공간)

// Function

	// 아이템을 추가한다
	void AddItem(const FName& Name);
	// 아이템을 추가한다
	void AddItem(EItemCode ItemCode);
	// 특정 크기의 아이템이 들어갈 공간이 존재한다면 true를 반환한다
	bool IsEmptySlot(const FIntPoint& Scale);
	// 입력된 크기와 위치에 아이템을 놓을 수 있다면 true를 반환한다
	bool IsEmptySlot(const FIntPoint& Pos, const FIntPoint& Scale);
	// 아이템이 해당 슬롯에 놓을 수 있다면 true를 반환한다
	bool IsEmptySlot(const UBInventorySlot* Slot, const UBInventoryItem* Item);
	// 아이템을 들어올린다
	void RaiseItem(UBInventoryItem* Item);
	// 아이템을 이동
	void MoveItem(UBInventoryItem* Item, const UBInventorySlot* Slot);
	// 아이템 이동을 확정짓는다
	void MoveItemConfirm(UBInventoryItem* Item, const UBInventorySlot* Slot);
	// 아이템을 교환할 수 있는지 여부를 알아낸다
	bool CheckChange(UBInventoryItem* Item, const UBInventorySlot* Slot);
	// 현재 들고있는 아이템을 내려 놓고 그 자리에 있는 아이템을 들어올린다
	UBInventoryItem* ChangeItem(UBInventoryItem* Item, const UBInventorySlot* Slot);
	// 특정 크기의 아이템이 들어갈 공간이 있다면 그 위치를 반환한다. 아이템이 들어갈 공간이 없다면 FIntPoint::NoneValue를 리턴
	FIntPoint FindEmptySlot(const FIntPoint& Scale);
	// 해당 위치에 아이템의 포인터를 받는다
	UBInventoryItem* FindItem(const UBInventorySlot* Slot);
	// 해당 위치에 아이템의 포인터를 받는다
	UBInventoryItem* FindItem(const FIntPoint& Pos, bool IsSubSlot = false);
	// 해당 범위에서 아이템을 찾아서 리턴한다
	UBInventoryItem* FindItemRange(const FIntPoint& Pos, const FIntPoint& Size, bool IsSubSlot = false);
	// 해당 위치가 유효한 슬롯인지 검사합니다
	bool IsVaildSlot(const FIntPoint& Pos, bool IsSubSlot = false);
	// 해당 위치가 유효한 슬롯인지 검사합니다
	bool IsVaildSlot(const UBInventorySlot* Slot);
	// 해당 범위가 유효한 슬롯인지 검사합니다
	bool IsVaildSlotRange(const UBInventorySlot* Slot, const FIntPoint& Size);
	// 해당 슬롯에 아이템이 있거나 범위 밖이면 false를 리턴한다 (유효한 슬롯일 시 true)
	bool CheckSlot(const FIntPoint& Pos);
	// 해당 서브슬롯에 아이템이 있거나 범위 밖이면 false를 리턴한다, IgnoreItem이 곂친 경우에는 무시한다
	bool CheckSubSlot(const FIntPoint& Pos);
	// 해당 범위에 아이템이 있거나 범위 밖이면 false를 리턴한다 (유효한 슬롯일 시 true)
	bool CheckSlotRange(const FIntPoint& Pos, const FIntPoint& Size);
	// 서브 슬롯에 아이템이 있다면 true를 리턴한다
	bool HasItemInSubSlot();

private:
	// 아이템을 추가한다
	void CreateItem(const FBItemData& Data);
	// 아이템을 배치한다
	void PlaceItemSlot(UBInventoryItem* Item, const FIntPoint& Pos);
	// 아이템을 배치한다
	void PlaceItemSlot(UBInventoryItem* Item, const UBInventorySlot* Slot);
	// 아이템의 기존 위치에 있는 슬롯을 정리한다
	void ClearSlot(const FIntPoint& Location, const FIntPoint& Size, bool IsSubSlot = false);

};
