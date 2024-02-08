// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"
#include "InventoryActor.h"
#include "InventoryWeapon.h"
#include "InventorySlot.h"
#include "BiohazardRe4.h"

UBInventoryManager* UBInventoryManager::Instance = nullptr;

UBInventoryManager::UBInventoryManager()
{
	Instance = this;
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

const FBItemData& UBInventoryManager::FindItemData(EItemCode Code)
{
	TArray<FName> RowNames = ItemDataTable->GetRowNames();
	for (FName Name : RowNames)
	{
		FBItemData* ItemData = ItemDataTable->FindRow<FBItemData>(Name, "");
		if (ItemData->ItemCode == Code)
		{
			return *ItemData;
		}
	}
	LOG_ERROR(TEXT("Can not find ItemData"));
	return *ItemDataTable->FindRow<FBItemData>(TEXT("HandgunAmmo"), TEXT(""));
}

void UBInventoryManager::AddItem(EItemCode ItemCode, int Num)
{
	TArray<FName> RowNames = ItemDataTable->GetRowNames();
	FBItemData* FindItem = nullptr;

	for (FName Name : RowNames)
	{
		FBItemData* ItemData = ItemDataTable->FindRow<FBItemData>(Name, "");
		if (ItemData->ItemCode == ItemCode)
		{
			FindItem = ItemData;
			break;
		}
	}

	if (nullptr == FindItem)
	{
		LOG_ERROR(TEXT("Can't Find ItemCode in ItemData"));
		return;
	}

	CreateItem(*FindItem, Num);
}

void UBInventoryManager::AddItem(FName ItemRowName, int Num)
{
	FBItemData* ItemData = ItemDataTable->FindRow<FBItemData>(ItemRowName, TEXT("Can't Find"));
	CreateItem(*ItemData, Num);
}

void UBInventoryManager::AddItem(EItemCode ItemCode, const FIntPoint& Pos, int Num)
{
	TArray<FName> RowNames = ItemDataTable->GetRowNames();
	FBItemData* FindItem = nullptr;

	for (FName Name : RowNames)
	{
		FBItemData* ItemData = ItemDataTable->FindRow<FBItemData>(Name, "");
		if (ItemData->ItemCode == ItemCode)
		{
			FindItem = ItemData;
			break;
		}
	}

	if (nullptr == FindItem)
	{
		LOG_ERROR(TEXT("Can't Find ItemCode in ItemData"));
		return;
	}

	CreateItem(*FindItem, Pos, Num);
}

void UBInventoryManager::RemoveItem(EItemCode ItemCode, int Num)
{
	int RemoveNum = Num;
	TArray<ABInventoryItem*> Items;
	ItemMap.MultiFind(ItemCode, Items, true);
	
	for (ABInventoryItem* Item : Items)
	{
		if (Item->Count <= RemoveNum)
		{
			RemoveNum -= Item->Count;
			ClearSlot(Item->GetItemPosition(), Item->GetItemSize(), Item->IsSubSlot());
			ItemMap.Remove(ItemCode, Item);
			Item->Destroy();
		}
		else
		{
			Item->Count -= RemoveNum;
			RemoveNum = 0;
			Item->SetItemNumText();
			break;
		}
	}

	if (0 < RemoveNum)
	{
		LOG_ERROR(TEXT("RemoveItem Error. Item Lack"))
	}
}

void UBInventoryManager::RemoveItem(ABInventoryItem* Item, int Num)
{
	int RemoveNum = Num;
	RemoveNum -= Item->Count;
	Item->Count -= Num;
	Item->SetItemNumText();

	if (Item->Count <= 0)
	{
		ClearSlot(Item->GetItemPosition(), Item->GetItemSize(), Item->IsSubSlot());
		ItemMap.Remove(Item->GetData().ItemCode, Item);
		Item->Destroy();
	}
	if (0 < RemoveNum)
	{
		RemoveItem(Item->GetData().ItemCode, RemoveNum);
	}
}

void UBInventoryManager::CraftItem(const FBCraftRecipe& Recipe)
{
	ABInventoryItem* SelectItem = ABInventoryActor::Instance->SelectItem;
	FIntPoint Pos = SelectItem->GetItemPosition();

	if (Recipe.AItem == SelectItem->GetData().ItemCode)
	{
		RemoveItem(SelectItem, Recipe.ANum);
		RemoveItem(Recipe.BItem, Recipe.BNum);
	}
	else
	{
		RemoveItem(SelectItem, Recipe.BNum);
		RemoveItem(Recipe.AItem, Recipe.ANum);
	}

	if (IsEmptySlot(Pos, FindItemData(Recipe.ResultItem).ItemSize))
	{
		AddItem(Recipe.ResultItem, Pos, Recipe.ResultNum);
	}
	else
	{
		AddItem(Recipe.ResultItem, Recipe.ResultNum);
	}
}

static int32 count = 0;
void UBInventoryManager::CreateItem(const FBItemData& Data, int Num)
{
	// 기존 아이템에 합칠 수 있는지 체크
	if (1 < Data.MaxCount)
	{
		Num = ItemMerge(Data, Num);
		if (Num <= 0)
		{
			return;
		}
	}
	// 아이템이 들어갈 공간 찾기
	FIntPoint Point = FindEmptySlot(Data.ItemSize);
	if (Point == FIntPoint::NoneValue)
	{
		// 못 찾은 경우
		LOG_WARNING(TEXT("Inventory Is Full. Check Please"));
		return;
	}

	ABInventoryItem* NewItem;
	switch (Data.ItemCode)
	{
	case EItemCode::Handgun_SR09R:
	case EItemCode::Shotgun_W870:
	case EItemCode::Rifle_SRM1903:
	{
		ABInventoryWeapon* Weapon = GetWorld()->SpawnActor<ABInventoryWeapon>(ABInventoryActor::Instance->WeaponClass);
		Weapon->SetLoadedAmmo(Data.MagazineCapacity);
		Weapon->SetMaxAmmo(Data.MagazineCapacity);
		NewItem = Cast<ABInventoryItem>(Weapon);
		break;
	}
	case EItemCode::CombatKnife:
	{
		ABInventoryWeapon* Weapon = GetWorld()->SpawnActor<ABInventoryWeapon>(ABInventoryActor::Instance->KnifeClass);
		Weapon->SetDurability(Data.MagazineCapacity);
		Weapon->SetMaxDurability(Data.MagazineCapacity);
		NewItem = Cast<ABInventoryItem>(Weapon);
		break;
	}
	default:
		if (1 == Data.MaxCount)
		{
			NewItem = GetWorld()->SpawnActor<ABInventoryItem>();
		}
		else
		{
			NewItem = GetWorld()->SpawnActor<ABInventoryItem>(ABInventoryActor::Instance->ItemClass);
		}
		break;
	}

	// 아이템 생성
	if (NewItem)
	{
		NewItem->Count = Num;
		NewItem->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		NewItem->SetItemData(Data);
		ItemMap.Add(TTuple<EItemCode, ABInventoryItem*>(Data.ItemCode, NewItem));
		PlaceItemSlot(NewItem, Point);
		NewItem->GetRootComponent()->SetRelativeLocation(FVector(Point.X * GridScale, Point.Y * GridScale, 0) + GridStart);
	}
}

void UBInventoryManager::CreateItem(const FBItemData& Data, const FIntPoint& Pos, int Num)
{
	// 기존 아이템에 합칠 수 있는지 체크
	if (1 < Data.MaxCount)
	{
		Num = ItemMerge(Data, Num);
		if (Num <= 0)
		{
			return;
		}
	}

	// 아이템이 들어갈 공간 찾기
	if (false == IsEmptySlot(Pos, Data.ItemSize))
	{
		LOG_ERROR(TEXT("CreateItem Faild"));
		return;
	}

	// 기존 아이템에 개수를 추가하는 경우는 아직 미구현
	ABInventoryItem* NewItem;

	switch (Data.ItemCode)
	{
	case EItemCode::Handgun_SR09R:
	case EItemCode::Shotgun_W870:
	case EItemCode::Rifle_SRM1903:
	{
		ABInventoryWeapon* Weapon = GetWorld()->SpawnActor<ABInventoryWeapon>(ABInventoryActor::Instance->WeaponClass);
		Weapon->SetLoadedAmmo(Data.MagazineCapacity);
		Weapon->SetMaxAmmo(Data.MagazineCapacity);
		NewItem = Cast<ABInventoryItem>(Weapon);
		break;
	}
	case EItemCode::CombatKnife:
	{
		ABInventoryWeapon* Weapon = GetWorld()->SpawnActor<ABInventoryWeapon>(ABInventoryActor::Instance->KnifeClass);
		Weapon->SetDurability(Data.MagazineCapacity);
		Weapon->SetMaxDurability(Data.MagazineCapacity);
		NewItem = Cast<ABInventoryItem>(Weapon);
		break;
	}
	default:
		if (1 == Data.MaxCount)
		{
			NewItem = GetWorld()->SpawnActor<ABInventoryItem>();
		}
		else
		{
			NewItem = GetWorld()->SpawnActor<ABInventoryItem>(ABInventoryActor::Instance->ItemClass);
		}
		break;
	}

	// 아이템 생성
	if (NewItem)
	{
		NewItem->Count = 1;
		NewItem->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		NewItem->SetItemData(Data);
		ItemMap.Add(TTuple<EItemCode, ABInventoryItem*>(Data.ItemCode, NewItem));
		PlaceItemSlot(NewItem, Pos);
		NewItem->GetRootComponent()->SetRelativeLocation(FVector(Pos.X * GridScale, Pos.Y * GridScale, 0) + GridStart);
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

bool UBInventoryManager::IsEmptySlot(EItemCode ItemCode, int Count)
{
	TArray<FName> RowNames = ItemDataTable->GetRowNames();
	FBItemData* Data = nullptr;

	for (FName Name : RowNames)
	{
		FBItemData* ItemData = ItemDataTable->FindRow<FBItemData>(Name, "");
		if (ItemData->ItemCode == ItemCode)
		{
			Data = ItemData;
			break;
		}
	}

	// 기존 아이템에 합칠 수 있는지 체크
	if (1 < Data->MaxCount)
	{
		Count = ItemMergeCheck(*Data, Count);
		if (Count <= 0)
		{
			// 공간이 남음
			return true;
		}
	}

	return IsEmptySlot(Data->ItemSize);
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

bool UBInventoryManager::IsEmptySlot(const UBInventorySlot* Slot, const ABInventoryItem* Item)
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

void UBInventoryManager::RaiseItem(ABInventoryItem* Item)
{
	ClearSlot(Item->GetItemPosition(), Item->GetItemSize(), Item->IsSubSlot());
	Item->SetRaise();
}

void UBInventoryManager::MoveItem(ABInventoryItem* Item, const UBInventorySlot* Slot)
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

void UBInventoryManager::MoveItemConfirm(ABInventoryItem* Item, const UBInventorySlot* Slot)
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

bool UBInventoryManager::CheckChange(ABInventoryItem* Item, const UBInventorySlot* Slot)
{
	if (false == IsVaildSlotRange(Slot, Item->GetItemSize()))
	{
		return false;
	}

	ABInventoryItem* CheckItem = nullptr;
	FIntPoint Pos = Slot->GetPosition();

	for (int y = 0; y < Item->GetItemSize().Y; y++)
	{
		for (int x = 0; x < Item->GetItemSize().X; x++)
		{
			if (ABInventoryItem* FItem = FindItem(Pos + FIntPoint(x, y), Slot->IsSubSlot()))
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

int UBInventoryManager::ItemMerge(const FBItemData& Data, int Num)
{
	TArray<ABInventoryItem*> Items;
	ItemMap.MultiFind(Data.ItemCode, Items, true);
	for (ABInventoryItem* Item : Items)
	{
		if (Item->Count < Data.MaxCount)
		{
			// 아이템을 더 넣을 수 있음
			int EmptyNum = Data.MaxCount - Item->Count;		// 빈공간
			if (Num <= EmptyNum)
			{
				// 남은 수를 모두 넣을 수 있다면
				Item->AddCount(Num);
				return 0;
			}
			else
			{
				// 공간이 부족한 경우
				Num -= EmptyNum;
				Item->SetCount(Data.MaxCount);
			}
		}
	}
	return Num;
}

int UBInventoryManager::ItemMergeCheck(const FBItemData& Data, int Num)
{
	TArray<ABInventoryItem*> Items;
	ItemMap.MultiFind(Data.ItemCode, Items, true);
	for (ABInventoryItem* Item : Items)
	{
		if (Item->Count < Data.MaxCount)
		{
			// 아이템을 더 넣을 수 있음
			int EmptyNum = Data.MaxCount - Item->Count;		// 빈공간
			if (Num <= EmptyNum)
			{
				// 남은 수를 모두 넣을 수 있다면
				return 0;
			}
			else
			{
				// 공간이 부족한 경우
				Num -= EmptyNum;
			}
		}
	}
	return Num;
}


ABInventoryItem* UBInventoryManager::ChangeItem(ABInventoryItem* Item, const UBInventorySlot* Slot)
{
	//ABInventoryItem* ChangeItem = FindItemRange(Pos, Item->GetItemSize());
	ABInventoryItem* ChangeItem = FindItemRange(Slot->GetPosition(), Item->GetItemSize(), Slot->IsSubSlot());
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

ABInventoryWeapon* UBInventoryManager::FindWeapon(EItemCode ItemCode)
{
	if (false == ItemMap.Contains(ItemCode))
	{
		return nullptr;
	}
	ABInventoryWeapon* Weapon = Cast<ABInventoryWeapon>(ItemMap.Find(ItemCode)[0]);
	return Weapon;
}

ABInventoryItem* UBInventoryManager::FindItem(const UBInventorySlot* Slot)
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

ABInventoryItem* UBInventoryManager::FindItem(const FIntPoint& Pos, bool IsSubSlot)
{
	if (IsSubSlot)
	{
		return SubSlot[Pos.Y * SubCaseSize.X + Pos.X]->GetItem();
	}
	return MainSlot[Pos.Y * CaseSize.X + Pos.X]->GetItem();
}

ABInventoryItem* UBInventoryManager::FindItemRange(const FIntPoint& Pos, const FIntPoint& Size, bool IsSubSlot)
{
	for (int y = 0; y < Size.Y; y++)
	{
		for (int x = 0; x < Size.X; x++)
		{
			FIntPoint FindPos = Pos + FIntPoint(x, y);
			if (ABInventoryItem* FItem = FindItem(FindPos, IsSubSlot))
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

void UBInventoryManager::PlaceItemSlot(ABInventoryItem* Item, const FIntPoint& Pos)
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

void UBInventoryManager::PlaceItemSlot(ABInventoryItem* Item, const UBInventorySlot* Slot)
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

	ABInventoryItem* Item = SubSlot[Pos.Y * SubCaseSize.X + Pos.X]->GetItem();
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

FVector UBInventoryManager::GetItemWorldLocation(ABInventoryItem* Item)
{
	FIntPoint Pos = Item->GetItemPosition();
	if (Item->IsSubSlot())
	{
		return SubSlot[Pos.Y * SubCaseSize.X + Pos.X]->GetComponentLocation();
	}
	return MainSlot[Pos.Y * CaseSize.X + Pos.X]->GetComponentLocation();
}

int UBInventoryManager::GetItemCount(EItemCode Code) const 
{
	int Num = 0;
	TArray<ABInventoryItem*> Items;
	ItemMap.MultiFind(Code, Items, true);

	for (ABInventoryItem* Item : Items)
	{
		Num += Item->Count;
	}

	return Num;
}

void UBInventoryManager::RemoveAllItemInSubSlot()
{
	for (int y = 0; y < SubCaseSize.Y; y++)
	{
		for (int x = 0; x < SubCaseSize.X; x++)
		{
			if (ABInventoryItem* Item = SubSlot[y * SubCaseSize.X + x]->GetItem())
			{
				ItemMap.Remove(Item->GetData().ItemCode, Item);
				SubSlot[y * SubCaseSize.X + x]->SetItem(nullptr);
				Item->Destroy();
			}
		}
	}
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
