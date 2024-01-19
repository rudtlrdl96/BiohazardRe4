// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ItemData.h"
#include "InventoryItem.generated.h"

#define TO_KEY(EnumValue) static_cast<int32>(EnumValue)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIOHAZARDRE4_API UBInventoryItem : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBInventoryItem();

	// 아이템 데이터를 지정한다
	void SetItemData(const FBItemData& _Data);
	// 아이템을 이동시키기 전에 들어올린다
	void SetRaise();
	// 지정 Locataion으로 아이템을 이동시킨다
	void SetMove(const FVector& _Location);
	// 아이템 이동이 끝나 제자리에 놓는다
	void SetPut(const FVector& _Location);

	void Turn();
	inline uint8 GetIsTurn()
	{
		return bIsTurn;
	}

	inline void SetItemPosition(const FIntPoint& _Pos) { ItemPosition = _Pos; }
	inline void SetIsSubSlot(bool IsSubSlot) { bIsSubSlot = IsSubSlot; }
	inline const FBItemData& GetData() const { return Data; }
	inline const FIntPoint GetItemSize() const { return bIsTurn ? FIntPoint(Data.ItemSize.Y, Data.ItemSize.X) : Data.ItemSize; }
	inline const FIntPoint& GetItemPosition() const { return ItemPosition; }
	inline bool IsSubSlot() const { return bIsSubSlot; }
	inline const FName& GetItemName() const { return Data.ItemName; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	const float MoveSpeed = 15.0f;
	const float RaiseSpeed = 5.0f;
	const float TurnSpeed = 5.0f;

private:

	enum class ItemState
	{
		Wait,
		Move,
		Put
	};

	class UBFsm* FSMComp;

	FVector StartLocation;
	FVector TargetLocation;
	FVector MeshLocation;
	FVector MeshStartLocation;
	FVector MeshTargetLocation;
	FRotator MeshStartRotate;
	FRotator MeshTargetRotate;
	float MoveAlpha = 0;
	float RaiseAlpha = 0;
	float TurnAlpha = 0;

	UPROPERTY()
	FIntPoint ItemPosition;	// 아이템의 위치 (인벤토리상 위치)
	uint8 bIsTurn : 1;		// 아이템 회전 여부
	uint8 bIsCurrentTurn : 1;		// 아이템 회전 여부
	uint8 bIsSubSlot : 1;		// 아이템 회전 여부
	UPROPERTY()
	int32 Count;		// 아이템의 개수

	UPROPERTY()
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	FBItemData Data;	// 아이템 데이터

	void MoveEnter();
	void MoveUpdate(float DeltaTime);

	void PutEnter();
	void PutUpdate(float DeltaTime);

};
