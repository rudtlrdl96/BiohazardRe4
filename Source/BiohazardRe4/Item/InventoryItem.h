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

	// ������ �����͸� �����Ѵ�
	void SetItemData(const FBItemData& _Data);
	// �������� �̵���Ű�� ���� ���ø���
	void SetRaise();
	// ���� Locataion���� �������� �̵���Ų��
	void SetMove(const FVector& _Location);
	// ������ �̵��� ���� ���ڸ��� ���´�
	void SetPut(const FVector& _Location);

	inline void SetItemPosition(const FIntPoint& _Pos) { ItemPosition = _Pos; }
	inline const FBItemData& GetData() const { return Data; }
	inline const FIntPoint& GetItemSize() const { return Data.ItemSize; }
	inline const FIntPoint& GetItemPosition() const { return ItemPosition; }
	inline const FName& GetItemName() const { return Data.ItemName; }

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 10.0f;
	UPROPERTY(EditAnywhere)
	float RaiseSpeed = 5.0f;

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
	float MoveAlpha = 0;
	float RaiseAlpha = 0;

	UPROPERTY()
	FIntPoint ItemPosition;	// �������� ��ġ (�κ��丮�� ��ġ)

	UPROPERTY()
	int32 Count;		// �������� ����

	UPROPERTY()
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	FBItemData Data;	// ������ ������

	void MoveEnter();
	void MoveUpdate(float DeltaTime);

	void PutEnter();
	void PutUpdate(float DeltaTime);

};
