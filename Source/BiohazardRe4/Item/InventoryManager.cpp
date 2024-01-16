// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"
#include "InventoryItem.h"

UBInventoryManager::UBInventoryManager()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> Data(TEXT("/Script/Engine.DataTable'/Game/Assets/Data/Item/DT_Items.DT_Items'"));
	if (Data.Object)
	{
		ItemDataTable = Data.Object;
	}

	Slot.SetNum(CaseSize.Y);
	for (int i = 0; i < CaseSize.Y; i++)
	{
		Slot[i].SetNum(CaseSize.X);
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

	
	static int32 count = 0;
	UBInventoryItem* NewItem = NewObject<UBInventoryItem>(GetOwner(), UBInventoryItem::StaticClass(), FName(_Name.ToString() + FString::FromInt(count++)));

	if (NewItem)
	{
		NewItem->RegisterComponent();
		NewItem->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		NewItem->SetItemData(*Data);
		GetOwner()->RegisterAllComponents();
		Items.Push(NewItem);
	}

	FIntPoint Point = FindEmptySlot(Data->Scale);

	if (Point == FIntPoint::NoneValue)
	{
 		return;
	}

	PlaceItemSlot(NewItem, Point);
	NewItem->SetRelativeLocation(FVector(Point.X * GridScale, Point.Y * GridScale, 0) + GridStart);
	NewItem->SetRelativeRotation({ 0, 90, 0 });
}

FIntPoint UBInventoryManager::FindEmptySlot(const FIntPoint& _Scale)
{
	// ºó °ø°£ Ã£±â
	for (int y = 0; y <= CaseSize.Y - _Scale.Y; y++)
	{
		for (int x = 0; x <= CaseSize.X - _Scale.X; x++)
		{
			bool placed = false;
			for (int i = 0; i < _Scale.Y; i++)
			{
				for (int j = 0; j < _Scale.X; j++)
				{
					if (false == Slot[y + i][x + j].IsEmpty())
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
			Slot[_Location.Y + y][_Location.X + x].SetItem(_Item);
		}
	}
}
