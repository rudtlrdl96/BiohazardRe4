// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetMain.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "InventoryActor.h"

void UBInventoryWidgetMain::NativeOnInitialized()
{
	WarningPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Panel")));
	WarningText = Cast<UTextBlock>(GetWidgetFromName(TEXT("WarningText")));
}

void UBInventoryWidgetMain::SetItemData(const FBItemData& Data)
{
	ItemName = Data.ItemName.ToString();
	ItemInformation = Data.ItemInformation;
}

void UBInventoryWidgetMain::ClearItemData()
{
	ItemName = "";
	ItemInformation = "";
}

void UBInventoryWidgetMain::OkButtonEvent()
{
	OkEvent.Execute();
}

void UBInventoryWidgetMain::NoButtonEvent()
{
	NoEvent.Execute();
}

void UBInventoryWidgetMain::DropItem()
{
	WarningText->SetText(NSLOCTEXT("UI", "DropCheck", "한번 버린 아이템은 다시 주울 수 없습니다.\n\n이 아이템을 버리시겠습니까?"));
	{
		OkEvent.Unbind();
		OkEvent.BindUFunction(ABInventoryActor::Instance, TEXT("CompleteDrop"));
	}
	{
		NoEvent.Unbind();
		NoEvent.BindUFunction(ABInventoryActor::Instance, TEXT("DropCancel"));
	}
}

void UBInventoryWidgetMain::CloseCheck()
{
	WarningText->SetText(NSLOCTEXT("UI", "CloseCheck", "아타셰케이스 밖의 아이템을 모두 버리시겠습니까?"));
	{
		OkEvent.Unbind();
		OkEvent.BindUFunction(ABInventoryActor::Instance, TEXT("CloseInventory"));
	}
	{
		NoEvent.Unbind();
		NoEvent.BindUFunction(ABInventoryActor::Instance, TEXT("CloseCancel"));
	}
}
