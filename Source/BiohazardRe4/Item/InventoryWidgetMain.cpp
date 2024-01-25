// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetMain.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "InventoryActor.h"
void UBInventoryWidgetMain::NativeOnInitialized()
{
	WarningPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("WarningPanel")));
	WarningText = Cast<UTextBlock>(GetWidgetFromName(TEXT("WarningText")));
	OkButton = Cast<UButton>(GetWidgetFromName(TEXT("OkButton")));
	NoButton = Cast<UButton>(GetWidgetFromName(TEXT("NoButton")));
}

void UBInventoryWidgetMain::SetItemData(const FBItemData& Data)
{
	ItemName = Data.ItemName.ToString();
	ItemInformation = Data.ItemInformation;
}

inline void UBInventoryWidgetMain::ClearItemData()
{
	ItemName = "";
	ItemInformation = "";
}

void UBInventoryWidgetMain::OnDropItem()
{
	WarningText->SetText(NSLOCTEXT("UI", "DropCheck", "한번 버린 아이템은 다시 주울 수 없습니다.\n\n이 아이템을 버리시겠습니까?"));
	{
		FScriptDelegate Delegate;
		Delegate.BindUFunction(this, TEXT("Drop"));
		OkButton->OnClicked.Clear();
		OkButton->OnClicked.Add(Delegate);
	}
	{
		FScriptDelegate Delegate;
		Delegate.BindUFunction(this, TEXT("OffDropItem"));
		NoButton->OnClicked.Clear();
		NoButton->OnClicked.Add(Delegate);
	}
	WarningPanel->SetVisibility(ESlateVisibility::Visible);
}

void UBInventoryWidgetMain::OffDropItem()
{
	WarningPanel->SetVisibility(ESlateVisibility::Hidden);
	ABInventoryActor::Instance->DropCancel();
}

void UBInventoryWidgetMain::OffCloseCheck()
{
	WarningPanel->SetVisibility(ESlateVisibility::Hidden);
	ABInventoryActor::Instance->CloseCancel();
}

void UBInventoryWidgetMain::CloseCheck()
{
	WarningText->SetText(NSLOCTEXT("UI", "CloseCheck", "아타셰케이스 밖의 아이템을 모두 버리시겠습니까?"));
	{
		FScriptDelegate Delegate;
		Delegate.BindUFunction(ABInventoryActor::Instance, TEXT("CloseInventory"));
		OkButton->OnClicked.Clear();
		OkButton->OnClicked.Add(Delegate);
	}
	{
		FScriptDelegate Delegate;
		Delegate.BindUFunction(this, TEXT("OffCloseCheck"));
		NoButton->OnClicked.Clear();
		NoButton->OnClicked.Add(Delegate);
	}
	WarningPanel->SetVisibility(ESlateVisibility::Visible);
}

void UBInventoryWidgetMain::Drop()
{
	OffDropItem();
	ABInventoryActor::Instance->CompleteDrop();
}
