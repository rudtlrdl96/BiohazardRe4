// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"
#include "InventoryItem.h"
#include "InventorySlot.h"

UBInventoryManager::UBInventoryManager()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> Data(TEXT("/Script/Engine.DataTable'/Game/Assets/Data/Item/DT_Items.DT_Items'"));
	if (Data.Object)
	{
		ItemDataTable = Data.Object;
	}


	
}

void UBInventoryManager::BeginPlay()
{
	Super::BeginPlay();
}

void UBInventoryManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBInventoryManager::AddItem(const FName& _Name)
{
	FBItemData* Data = ItemDataTable->FindRow<FBItemData>(_Name, FString("Fail Find ItemData"));

	if (nullptr == Data)
		return;

	FIntPoint Point = FindEmptySlot(Data->Scale);
	if (Point == FIntPoint::NoneValue)
	{
		return;
	}

	static int32 count = 0;
	UBInventoryItem* NewItem = NewObject<UBInventoryItem>(GetOwner(), UBInventoryItem::StaticClass(), FName(_Name.ToString() + FString::FromInt(count++)));

	if (NewItem)
	{
		NewItem->RegisterComponent();
		NewItem->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		NewItem->SetItemData(*Data);
		GetOwner()->RegisterAllComponents();
		Items.Push(NewItem);
		PlaceItemSlot(NewItem, Point);
		NewItem->SetRelativeLocation(FVector(Point.X * GridScale, Point.Y * GridScale, 0) + GridStart);
		NewItem->SetRelativeRotation({ 0, 90, 0 });
	}

}

bool UBInventoryManager::IsEmptySlot(const FIntPoint& _Scale)
{
	// 회전해야 들어가는 상황은 고려하지 않았음 추후 수정
	// 빈 공간 찾기
	for (int y = 0; y <= CaseSize.Y - _Scale.Y; y++)
	{
		for (int x = 0; x <= CaseSize.X - _Scale.X; x++)
		{
			bool placed = false;
			for (int i = 0; i < _Scale.Y; i++)
			{
				for (int j = 0; j < _Scale.X; j++)
				{
					if (Slot[(y + i) * CaseSize.X + x + j]->HasItem())
					{
						placed = true;
						break;
					}
				}
				if (true == placed)
				{
					break;
				}
			}

			if (false == placed)
			{
				return true;
			}
		}
	}
	return false;
}

FIntPoint UBInventoryManager::FindEmptySlot(const FIntPoint& _Scale)
{
	// 회전해야 들어가는 상황은 고려하지 않았음 추후 수정
	// 빈 공간 찾기
	for (int y = 0; y <= CaseSize.Y - _Scale.Y; y++)
	{
		for (int x = 0; x <= CaseSize.X - _Scale.X; x++)
		{
			bool placed = false;
			for (int i = 0; i < _Scale.Y; i++)
			{
				for (int j = 0; j < _Scale.X; j++)
				{
					if (Slot[(y + i) * CaseSize.X + x + j]->HasItem())
					{
						placed = true;
						break;
					}
				}
				if (true == placed)
				{
					break;
				}
			}
			
			if (false == placed)
			{
				return FIntPoint(x, y);
			}
		}
	}
	return FIntPoint::NoneValue;
}

void UBInventoryManager::PlaceItemSlot(UBInventoryItem* _Item, const FIntPoint& _Location)
{
	for (int y = 0; y < _Item->Data.Scale.Y; y++)
	{
		for (int x = 0; x < _Item->Data.Scale.X; x++)
		{
			Slot[(_Location.Y + y) * CaseSize.X + _Location.X + x]->SetItem(_Item);
		}
	}
}
