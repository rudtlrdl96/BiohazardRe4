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

	FIntPoint Point = FindEmptySlot(Data->ItemSize);
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

bool UBInventoryManager::IsEmptySlot(const FIntPoint& _Pos, const FIntPoint& _Scale)
{
	for (int y = 0; y < _Scale.Y; y++)
	{
		for (int x = 0; x < _Scale.X; x++)
		{
			if (false == CheckSlot(_Pos + FIntPoint(x, y)))
			{
				return false;
			}
		}
	}
	return true;
}

bool UBInventoryManager::IsEmptySlot(const FIntPoint& _Pos, const UBInventoryItem* _Item)
{
	for (int y = 0; y < _Item->GetItemSize().Y; y++)
	{
		for (int x = 0; x < _Item->GetItemSize().X; x++)
		{
			if (false == CheckSlot(_Pos + FIntPoint(x, y), _Item))
			{
				return false;
			}
		}
	}
	return true;
}

void UBInventoryManager::RaiseItem(UBInventoryItem* _Item)
{
	ClearSlot(_Item->GetItemPosition(), _Item->GetItemSize());
	_Item->SetRaise();
}

void UBInventoryManager::MoveItem(UBInventoryItem* _Item, const FIntPoint& _Pos)
{
	_Item->SetMove(CaseLocation(_Pos));
}

void UBInventoryManager::MoveItemConfirm(UBInventoryItem* _Item, const FIntPoint& _Pos)
{
	// 드래그를 때서 이동이 결정된 상황에서 실행
	// 아이템을 놓을 장소에 아이템이 한개 있다면 커서에 있는 아이템과 놓인 아이템을 교체한다
	// 

	if (IsEmptySlot(_Pos, _Item->GetItemSize()))
	{
		_Item->SetPut(CaseLocation(_Pos));
		PlaceItemSlot(_Item, _Pos);
	}
	else
	{
		_Item->SetPut(CaseLocation(_Item->GetItemPosition()));
		PlaceItemSlot(_Item, _Item->GetItemPosition());
	}
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

void UBInventoryManager::PlaceItemSlot(UBInventoryItem* _Item, const FIntPoint& _Pos)
{
	for (int y = 0; y < _Item->GetItemSize().Y; y++)
	{
		for (int x = 0; x < _Item->GetItemSize().X; x++)
		{
			Slot[(_Pos.Y + y) * CaseSize.X + _Pos.X + x]->SetItem(_Item);
		}
	}
	_Item->SetItemPosition(_Pos);
}

bool UBInventoryManager::CheckSlot(const FIntPoint& Pos)
{
	if (CaseSize.X <= Pos.X) { return false; }
	if (CaseSize.Y <= Pos.Y) { return false; }

	return false == Slot[Pos.Y * CaseSize.X + Pos.X]->HasItem();
}

bool UBInventoryManager::CheckSlot(const FIntPoint& Pos, const UBInventoryItem* IgnoreItem)
{
	if (CaseSize.X <= Pos.X) { return false; }
	if (CaseSize.Y <= Pos.Y) { return false; }

	UBInventoryItem* Item = Slot[Pos.Y * CaseSize.X + Pos.X]->GetItem();
	return Item == nullptr || Item == IgnoreItem;
}

void UBInventoryManager::ClearSlot(const FIntPoint& _Pos, const FIntPoint& Size)
{
	for (int y = 0; y < Size.Y; y++)
	{
		for (int x = 0; x < Size.X; x++)
		{
			Slot[(_Pos.Y + y) * CaseSize.X + _Pos.X + x]->SetItem(nullptr);
		}
	}
}
