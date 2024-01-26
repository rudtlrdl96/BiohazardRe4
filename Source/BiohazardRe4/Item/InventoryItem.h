// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "InventoryItem.generated.h"

#define TO_KEY(EnumValue) static_cast<int32>(EnumValue)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIOHAZARDRE4_API ABInventoryItem : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ABInventoryItem();

	// ������ �����͸� �����Ѵ�
	void SetItemData(const FBItemData& _Data);
	// �������� �̵���Ű�� ���� ���ø���
	void SetRaise();
	// ���� Locataion���� �������� �̵���Ų��
	void SetMove(const FVector& _Location);
	// ������ �̵��� ���� ���ڸ��� ���´�
	void SetPut(const FVector& _Location);
	// �������� ������ �����Ѵ�

	// �������� �� ǥ�ø� �����Ѵ�
	UFUNCTION(BlueprintImplementableEvent)
	void SetItemNumText();
	UFUNCTION(BlueprintImplementableEvent)
	void OffItemNumText();

	void SetCount(int _Count)
	{
		Count = _Count;
		SetItemNumText();
	}

	void AddCount(int _Count)
	{
		Count += _Count;
		SetItemNumText();
	}

	void Turn();
	inline uint8 GetIsTurn()
	{
		return bIsTurn;
	}

	inline void SetItemPosition(const FIntPoint& _Pos) { ItemPosition = _Pos; }
	inline void SetIsSubSlot(bool IsSubSlot) { bIsSubSlot = IsSubSlot; }
	inline const FBItemData& GetData() const { return Data; }
	UFUNCTION(BlueprintCallable)
	inline int GetMaxCount() const { return Data.MaxCount; }
	UFUNCTION(BlueprintCallable)
	inline FIntPoint GetItemSize() const { return bIsTurn ? FIntPoint(Data.ItemSize.Y, Data.ItemSize.X) : Data.ItemSize; }
	inline const FIntPoint& GetItemPosition() const { return ItemPosition; }
	inline bool IsSubSlot() const { return bIsSubSlot; }
	inline const FName& GetItemName() const { return Data.ItemName; }
	inline FVector GetMeshLocation() const { return Mesh->GetComponentLocation(); }

	virtual void Tick(float DeltaTime) override;

	const float MoveSpeed = 15.0f;
	const float RaiseSpeed = 5.0f;
	const float TurnSpeed = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Count = 1;		// �������� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

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
	FIntPoint ItemPosition;	// �������� ��ġ (�κ��丮�� ��ġ)
	uint8 bIsTurn : 1;		// ������ ȸ�� ����
	uint8 bIsCurrentTurn : 1;		// ������ ȸ�� ����
	uint8 bIsSubSlot : 1;		// ������ ȸ�� ����

	UPROPERTY()
	FBItemData Data;	// ������ ������

	void MoveEnter();
	void MoveUpdate(float DeltaTime);

	void PutEnter();
	void PutUpdate(float DeltaTime);

};
