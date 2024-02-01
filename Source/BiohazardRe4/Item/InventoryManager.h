// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CraftRecipe.h"
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

	static UBInventoryManager* Instance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;	// 데이터 테이블 (아이템 데이터베이스)

	TMultiMap<EItemCode, class ABInventoryItem*> ItemMap;		// 현재 지닌 모든 아이템들의 배열

	TArray<class UBInventorySlot*> MainSlot;	// 2차원 인벤토리 공간

	TArray<class UBInventorySlot*> SubSlot;	// 2차원 서브 인벤토리 공간 (버리는 공간)

// Function
	const FBItemData& FindItemData(EItemCode Code);
	// 아이템을 추가한다
	void AddItem(EItemCode ItemCode, int Num = 1);
	// 아이템을 해당 위치에 추가한다
	void AddItem(EItemCode ItemCode, const FIntPoint& Pos, int Num = 1);
	// 아이템을 제거한다. Num개 만큼 제거합니다
	void RemoveItem(EItemCode ItemCode, int Num = 1);
	// Item을 Num만큼 제거합니다. Item의 수가 부족하면 같은 종류의 다른 아이템을 제거합니다
	void RemoveItem(ABInventoryItem* Item, int Num = 1);
	// 아이템을 제작한다
	void CraftItem(const FBCraftRecipe& Recipe);
	// 특정 크기의 아이템이 들어갈 공간이 존재한다면 true를 반환한다
	bool IsEmptySlot(const FIntPoint& Scale);
	// 입력된 크기와 위치에 아이템을 놓을 수 있다면 true를 반환한다
	bool IsEmptySlot(const FIntPoint& Pos, const FIntPoint& Scale);
	// 아이템이 해당 슬롯에 놓을 수 있다면 true를 반환한다
	bool IsEmptySlot(const UBInventorySlot* Slot, const ABInventoryItem* Item);
	// 아이템을 들어올린다
	void RaiseItem(ABInventoryItem* Item);
	// 아이템을 이동
	void MoveItem(ABInventoryItem* Item, const UBInventorySlot* Slot);
	// 아이템 이동을 확정짓는다
	void MoveItemConfirm(ABInventoryItem* Item, const UBInventorySlot* Slot);
	// 아이템을 교환할 수 있는지 여부를 알아낸다
	bool CheckChange(ABInventoryItem* Item, const UBInventorySlot* Slot);
	// 해당 아이템을 병합하고 남는 개수를 리턴한다
	int ItemMerge(const FBItemData& Data, int Num);
	// 현재 들고있는 아이템을 내려 놓고 그 자리에 있는 아이템을 들어올린다
	ABInventoryItem* ChangeItem(ABInventoryItem* Item, const UBInventorySlot* Slot);
	// 특정 크기의 아이템이 들어갈 공간이 있다면 그 위치를 반환한다. 아이템이 들어갈 공간이 없다면 FIntPoint::NoneValue를 리턴
	FIntPoint FindEmptySlot(const FIntPoint& Scale);
	// 해당 위치에 아이템의 포인터를 받는다
	ABInventoryItem* FindItem(const UBInventorySlot* Slot);
	// 해당 위치에 아이템의 포인터를 받는다
	ABInventoryItem* FindItem(const FIntPoint& Pos, bool IsSubSlot = false);
	// 해당 범위에서 아이템을 찾아서 리턴한다
	ABInventoryItem* FindItemRange(const FIntPoint& Pos, const FIntPoint& Size, bool IsSubSlot = false);
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
	// 해당 아이템에 월드 위치값을 받아옵니다
	FVector GetItemWorldLocation(ABInventoryItem* Item);
	// 해당 코드의 아이템이 인벤토리에 몇개 있는지 받아옵니다
	int GetItemCount(EItemCode Code);
	// 서브 슬롯에 있는 모든 아이템을 버립니다
	void RemoveAllItemInSubSlot();

private:
	// 아이템을 추가한다
	void CreateItem(const FBItemData& Data, int Num = 1);
	// 아이템을 해당 위치에 추가합니다
	void CreateItem(const FBItemData& Data, const FIntPoint& Pos, int Num = 1);
	// 아이템을 배치한다
	void PlaceItemSlot(ABInventoryItem* Item, const FIntPoint& Pos);
	// 아이템을 배치한다
	void PlaceItemSlot(ABInventoryItem* Item, const UBInventorySlot* Slot);
	// 아이템의 기존 위치에 있는 슬롯을 정리한다
	void ClearSlot(const FIntPoint& Location, const FIntPoint& Size, bool IsSubSlot = false);

};
