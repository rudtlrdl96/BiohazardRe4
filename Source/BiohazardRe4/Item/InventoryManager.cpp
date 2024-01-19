// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"
#include "InventoryItem.h"
#include "InventorySlot.h"
#include "BiohazardRe4.h"

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

void UBInventoryManager::AddItem(const FName& Name)
{
	if (nullptr == ItemDataTable)
	{
		LOG_WARNING(TEXT("ItemDataTable Not Setting."));
		return;
	}

	// 데이터 테이블에서 아이템 찾기
	FBItemData* Data = ItemDataTable->FindRow<FBItemData>(Name, FString("Fail Find ItemData"));

	if (nullptr == Data)
	{
		LOG_WARNING(TEXT("Can't Found Name in ItemDataTable"));
		return;
	}

	// 아이템이 들어갈 공간 찾기
	FIntPoint Point = FindEmptySlot(Data->ItemSize);
	if (Point == FIntPoint::NoneValue)
	{
		// 못 찾은 경우
		LOG_WARNING(TEXT("Inventory Is Full. Check Please"));
		return;
	}

	static int32 count = 0;
	UBInventoryItem* NewItem = NewObject<UBInventoryItem>(GetOwner(), UBInventoryItem::StaticClass(), FName(Name.ToString() + FString::FromInt(count++)));

	// 아이템 생성
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

bool UBInventoryManager::IsEmptySlot(const FIntPoint& Scale)
{
	// 회전해야 들어가는 상황은 고려하지 않았음 추후 수정
	// 크기 만큼 비어있는 공간을 찾는 과정
	for (int y = 0; y <= CaseSize.Y - Scale.Y; y++)
	{
		for (int x = 0; x <= CaseSize.X - Scale.X; x++)
		{
			bool placed = false;
			for (int i = 0; i < Scale.Y; i++)
			{
				for (int j = 0; j < Scale.X; j++)
				{
					if (MainSlot[(y + i) * CaseSize.X + x + j]->HasItem())
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

bool UBInventoryManager::IsEmptySlot(const FIntPoint& Pos, const FIntPoint& Scale)
{
	for (int y = 0; y < Scale.Y; y++)
	{
		for (int x = 0; x < Scale.X; x++)
		{
			if (false == CheckSlot(Pos + FIntPoint(x, y)))
			{
				return false;
			}
		}
	}
	return true;
}

bool UBInventoryManager::IsEmptySlot(const UBInventorySlot* Slot, const UBInventoryItem* Item)
{
	if (false == IsVaildSlotRange(Slot, Item->GetItemSize()))
	{
		return false;
	}

	if (Slot->IsSubSlot())
	{
		for (int y = 0; y < Item->GetItemSize().Y; y++)
		{
			for (int x = 0; x < Item->GetItemSize().X; x++)
			{
				if (false == CheckSubSlot(Slot->GetPosition() + FIntPoint(x, y)))
				{
					return false;
				}
			}
		}
		return true;
	}

	for (int y = 0; y < Item->GetItemSize().Y; y++)
	{
		for (int x = 0; x < Item->GetItemSize().X; x++)
		{
			if (false == CheckSlot(Slot->GetPosition() + FIntPoint(x, y)))
			{
				return false;
			}
		}
	}
	return true;
}

void UBInventoryManager::RaiseItem(UBInventoryItem* Item)
{
	ClearSlot(Item->GetItemPosition(), Item->GetItemSize(), Item->IsSubSlot());
	Item->SetRaise();
}

void UBInventoryManager::MoveItem(UBInventoryItem* Item, const UBInventorySlot* Slot)
{
	FIntPoint Pos = Slot->GetPosition();

	if (Slot->IsSubSlot())
	{
		Item->SetMove(SubCaseLocation(Pos));
	}
	else
	{
		Item->SetMove(CaseLocation(Pos));
	}
}

void UBInventoryManager::MoveItemConfirm(UBInventoryItem* Item, const UBInventorySlot* Slot)
{
	// 드래그를 때서 이동이 결정된 상황에서 실행
	// 아이템을 놓을 장소에 아이템이 한개 있다면 커서에 있는 아이템과 놓인 아이템을 교체한다
	// 
	if (IsEmptySlot(Slot, Item))
	{
		if (Slot->IsSubSlot())
		{
			Item->SetPut(SubCaseLocation(Slot->GetPosition()));
		}
		else
		{
			Item->SetPut(CaseLocation(Slot->GetPosition()));
		}
		PlaceItemSlot(Item, Slot);
	}
}

bool UBInventoryManager::CheckChange(UBInventoryItem* Item, const UBInventorySlot* Slot)
{
	if (false == IsVaildSlotRange(Slot, Item->GetItemSize()))
	{
		return false;
	}

	UBInventoryItem* CheckItem = nullptr;
	FIntPoint Pos = Slot->GetPosition();

	for (int y = 0; y < Item->GetItemSize().Y; y++)
	{
		for (int x = 0; x < Item->GetItemSize().X; x++)
		{
			if (UBInventoryItem* FItem = FindItem(Pos + FIntPoint(x, y), Slot->IsSubSlot()))
			{
				if (CheckItem == nullptr)
				{
					CheckItem = FItem;
				}
				else
				{
					if (CheckItem != FItem)
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}

UBInventoryItem* UBInventoryManager::ChangeItem(UBInventoryItem* Item, const UBInventorySlot* Slot)
{
	//UBInventoryItem* ChangeItem = FindItemRange(Pos, Item->GetItemSize());
	UBInventoryItem* ChangeItem = FindItemRange(Slot->GetPosition(), Item->GetItemSize(), Slot->IsSubSlot());
	if (nullptr == ChangeItem) { return nullptr; }
	RaiseItem(ChangeItem);

	if (Slot->IsSubSlot())
	{
		Item->SetPut(SubCaseLocation(Slot->GetPosition()));
	}
	else
	{
		Item->SetPut(CaseLocation(Slot->GetPosition()));
	}
	PlaceItemSlot(Item, Slot);

	return ChangeItem;
}

FIntPoint UBInventoryManager::FindEmptySlot(const FIntPoint& Scale)
{
	// 회전해야 들어가는 상황은 고려하지 않았음 추후 수정
	// 빈 공간 찾기
	for (int y = 0; y <= CaseSize.Y - Scale.Y; y++)
	{
		for (int x = 0; x <= CaseSize.X - Scale.X; x++)
		{
			bool placed = false;
			for (int i = 0; i < Scale.Y; i++)
			{
				for (int j = 0; j < Scale.X; j++)
				{
					if (MainSlot[(y + i) * CaseSize.X + x + j]->HasItem())
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

UBInventoryItem* UBInventoryManager::FindItem(const UBInventorySlot* Slot)
{
	if (false == IsVaildSlot(Slot))
	{
		return nullptr;
	}

	FIntPoint Pos = Slot->GetPosition();

	if (Slot->IsSubSlot())
	{
		return SubSlot[Pos.Y * SubCaseSize.X + Pos.X]->GetItem();
	}
	return MainSlot[Pos.Y * CaseSize.X + Pos.X]->GetItem();
}

UBInventoryItem* UBInventoryManager::FindItem(const FIntPoint& Pos, bool IsSubSlot)
{
	if (IsSubSlot)
	{
		return SubSlot[Pos.Y * SubCaseSize.X + Pos.X]->GetItem();
	}
	return MainSlot[Pos.Y * CaseSize.X + Pos.X]->GetItem();
}

UBInventoryItem* UBInventoryManager::FindItemRange(const FIntPoint& Pos, const FIntPoint& Size, bool IsSubSlot)
{
	for (int y = 0; y < Size.Y; y++)
	{
		for (int x = 0; x < Size.X; x++)
		{
			FIntPoint FindPos = Pos + FIntPoint(x, y);
			if (UBInventoryItem* FItem = FindItem(FindPos, IsSubSlot))
			{
				return FItem;
			}
		}
	}
	return nullptr;
}

bool UBInventoryManager::IsVaildSlot(const FIntPoint& Pos, bool IsSubSlot)
{
	if (IsSubSlot)
	{
		if (SubCaseSize.X <= Pos.X) { return false; }
		if (SubCaseSize.Y <= Pos.Y) { return false; }
		return true;
	}

	if (CaseSize.X <= Pos.X) { return false; }
	if (CaseSize.Y <= Pos.Y) { return false; }
	return true;
}

void UBInventoryManager::PlaceItemSlot(UBInventoryItem* Item, const FIntPoint& Pos)
{
	for (int y = 0; y < Item->GetItemSize().Y; y++)
	{
		for (int x = 0; x < Item->GetItemSize().X; x++)
		{
			MainSlot[(Pos.Y + y) * CaseSize.X + Pos.X + x]->SetItem(Item);
		}
	}
	Item->SetItemPosition(Pos);
	Item->SetIsSubSlot(false);
}

void UBInventoryManager::PlaceItemSlot(UBInventoryItem* Item, const UBInventorySlot* Slot)
{
	FIntPoint Pos = Slot->GetPosition();
	Item->SetItemPosition(Pos);

	if (Slot->IsSubSlot())
	{
		for (int y = 0; y < Item->GetItemSize().Y; y++)
		{
			for (int x = 0; x < Item->GetItemSize().X; x++)
			{
				SubSlot[(Pos.Y + y) * SubCaseSize.X + Pos.X + x]->SetItem(Item);
			}
		}
		Item->SetIsSubSlot(true);
		return;
	}

	for (int y = 0; y < Item->GetItemSize().Y; y++)
	{
		for (int x = 0; x < Item->GetItemSize().X; x++)
		{
			MainSlot[(Pos.Y + y) * CaseSize.X + Pos.X + x]->SetItem(Item);
		}
	}
	Item->SetIsSubSlot(false);
}

bool UBInventoryManager::IsVaildSlot(const UBInventorySlot* Slot)
{
	FIntPoint Pos = Slot->GetPosition();

	if (Slot->IsSubSlot())
	{
		if (SubCaseSize.X <= Pos.X) { return false; }
		if (SubCaseSize.Y <= Pos.Y) { return false; }
		return true;
	}
	
	if (CaseSize.X <= Pos.X) { return false; }
	if (CaseSize.Y <= Pos.Y) { return false; }
	return true;
}

bool UBInventoryManager::IsVaildSlotRange(const UBInventorySlot* Slot, const FIntPoint& Size)
{
	FIntPoint Pos = Slot->GetPosition();

	if (Slot->IsSubSlot())
	{
		for (int y = 0; y < Size.Y; y++)
		{
			for (int x = 0; x < Size.X; x++)
			{
				if (false == IsVaildSlot(Pos + FIntPoint(x, y), true))
				{
					return false;
				}
			}
		}
		return true;
	}

	for (int y = 0; y < Size.Y; y++)
	{
		for (int x = 0; x < Size.X; x++)
		{
			if (false == IsVaildSlot(Pos + FIntPoint(x, y), false))
			{
				return false;
			}
		}
	}
	return true;
}

bool UBInventoryManager::CheckSlot(const FIntPoint& Pos)
{
	if (CaseSize.X <= Pos.X) { return false; }
	if (CaseSize.Y <= Pos.Y) { return false; }

	return false == MainSlot[Pos.Y * CaseSize.X + Pos.X]->HasItem();
}

bool UBInventoryManager::CheckSubSlot(const FIntPoint& Pos)
{
	if (SubCaseSize.X <= Pos.X) { return false; }
	if (SubCaseSize.Y <= Pos.Y) { return false; }

	UBInventoryItem* Item = SubSlot[Pos.Y * SubCaseSize.X + Pos.X]->GetItem();
	return Item == nullptr;
}

bool UBInventoryManager::CheckSlotRange(const FIntPoint& Pos, const FIntPoint& Size)
{
	for (int y = 0; y < Size.Y; y++)
	{
		for (int x = 0; x < Size.X; x++)
		{
			if (false == CheckSlot(Pos + FIntPoint(x, y)))
			{
				return false;
			}
		}
	}
	return true;
}

bool UBInventoryManager::HasItemInSubSlot()
{
	for (int y = 0; y < SubCaseSize.Y; y++)
	{
		for (int x = 0; x < SubCaseSize.X; x++)
		{
			if (SubSlot[y * SubCaseSize.X + x]->HasItem())
			{
				return true;
			}
		}
	}
	return false;
}

void UBInventoryManager::ClearSlot(const FIntPoint& Pos, const FIntPoint& Size, bool IsSubSlot)
{
	if (IsSubSlot)
	{
		for (int y = 0; y < Size.Y; y++)
		{
			for (int x = 0; x < Size.X; x++)
			{
				SubSlot[(Pos.Y + y) * SubCaseSize.X + Pos.X + x]->SetItem(nullptr);
			}
		}
		return;
	}

	for (int y = 0; y < Size.Y; y++)
	{
		for (int x = 0; x < Size.X; x++)
		{
			MainSlot[(Pos.Y + y) * CaseSize.X + Pos.X + x]->SetItem(nullptr);
		}
	}
}
