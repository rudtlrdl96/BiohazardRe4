// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetBehavior.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "BiohazardRe4.h"
#include "InventoryActor.h"

void UBInventoryWidgetBehavior::NativeOnInitialized()
{
	Panels.SetNum(4);
	//SetPositionInViewport({ 500, 500 });
	Panels[0] = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_0")));
	Panels[1] = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_1")));
	Panels[2] = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_2")));
	Panels[3] = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_3")));

	Buttons.SetNum(4);
	Buttons[0] = Cast<UButton>(GetWidgetFromName(TEXT("Button_0")));
	Buttons[1] = Cast<UButton>(GetWidgetFromName(TEXT("Button_1")));
	Buttons[2] = Cast<UButton>(GetWidgetFromName(TEXT("Button_2")));
	Buttons[3] = Cast<UButton>(GetWidgetFromName(TEXT("Button_3")));
	
	Images.SetNum(4);
	Images[0] = Cast<UImage>(GetWidgetFromName(TEXT("Image_0")));
	Images[1] = Cast<UImage>(GetWidgetFromName(TEXT("Image_1")));
	Images[2] = Cast<UImage>(GetWidgetFromName(TEXT("Image_2")));
	Images[3] = Cast<UImage>(GetWidgetFromName(TEXT("Image_3")));

	Texts.SetNum(4);
	Texts[0] = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_0")));
	Texts[1] = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_1")));
	Texts[2] = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_2")));
	Texts[3] = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_3")));
}

void UBInventoryWidgetBehavior::SetItemData(const FBItemData& Data)
{

	// 공용 기능 = 조사, 버리기(보관고 보내기)
	switch (Data.ItemCode)
	{
	case EItemCode::Handgun_SR09R:
	case EItemCode::Shotgun_W870:
	case EItemCode::Rifle_SRM1903:
		SetButton(0, State_BehaviorButton::Equip);
		SetButton(1, State_BehaviorButton::Investigate);
		SetButton(2, State_BehaviorButton::Hotkey);
		SetButton(3, State_BehaviorButton::Drop);
		break;
	case EItemCode::CombatKnife:
	case EItemCode::HandgunAmmo:
	case EItemCode::ShotgunShells:
	case EItemCode::RifleAmmo:
		SetButton(0, State_BehaviorButton::Investigate);
		SetButton(1, State_BehaviorButton::Drop);
		SetButton(2, State_BehaviorButton::None);
		SetButton(3, State_BehaviorButton::None);
		// 조사하기, 보관고(버리기)
		break;
	case EItemCode::GreenHerb:
		// 사용하기, 조사하기, 제조하기, 보관고(버리기)
		SetButton(0, State_BehaviorButton::Use);
		SetButton(1, State_BehaviorButton::Investigate);
		SetButton(2, State_BehaviorButton::Crafting);
		SetButton(3, State_BehaviorButton::Drop);
		break;
	case EItemCode::MixedHerb_GG:
	case EItemCode::MixedHerb_GR:
	case EItemCode::FirstAidSpray:
		SetButton(0, State_BehaviorButton::Use);
		SetButton(1, State_BehaviorButton::Investigate);
		SetButton(2, State_BehaviorButton::Drop);
		SetButton(3, State_BehaviorButton::None);
		// 사용하기	
		break;
	case EItemCode::Gunpowder:
	case EItemCode::RedHerb:
	case EItemCode::Resources_S:
	case EItemCode::Resources_L:
		SetButton(0, State_BehaviorButton::Crafting);
		SetButton(1, State_BehaviorButton::Investigate);
		SetButton(2, State_BehaviorButton::Drop);
		SetButton(3, State_BehaviorButton::None);
		// 제조하기
		break;
	default:
		break;
	}
}

void UBInventoryWidgetBehavior::SetButton(int Index, State_BehaviorButton State)
{
	switch (State)
	{
	case UBInventoryWidgetBehavior::None:
		Panels[Index]->SetVisibility(ESlateVisibility::Hidden);
		return;
	case UBInventoryWidgetBehavior::Equip:
		Images[Index]->SetBrushResourceObject(Icons[0]);
		Texts[Index]->SetText(NSLOCTEXT("UI", "Equip", "장비하기"));
		{
			FScriptDelegate Delegate;
			Delegate.BindUFunction(this, FName("ButtonTest"));
			Buttons[Index]->OnClicked.Clear();
			Buttons[Index]->OnClicked.Add(Delegate);
		}
		break;
	case UBInventoryWidgetBehavior::Unequip:
		Images[Index]->SetBrushResourceObject(Icons[1]);
		Texts[Index]->SetText(NSLOCTEXT("UI", "Unequip", "해제하기"));
		break;
	case UBInventoryWidgetBehavior::Investigate:
		Images[Index]->SetBrushResourceObject(Icons[2]);
		Texts[Index]->SetText(NSLOCTEXT("UI", "Investigate", "조사하기"));
		break;
	case UBInventoryWidgetBehavior::Hotkey:
		Images[Index]->SetBrushResourceObject(Icons[3]);
		Texts[Index]->SetText(NSLOCTEXT("UI", "Hotkey", "단축키 등록"));
		break;
	case UBInventoryWidgetBehavior::Drop:
		Images[Index]->SetBrushResourceObject(Icons[4]);
		Texts[Index]->SetText(NSLOCTEXT("UI", "Drop", "버리기"));
		{
			FScriptDelegate Delegate;
			Delegate.BindUFunction(InventoryActor, FName("DropItem"));
			Buttons[Index]->OnClicked.Clear();
			Buttons[Index]->OnClicked.Add(Delegate);
		}
		break;
	case UBInventoryWidgetBehavior::Crafting:
		Images[Index]->SetBrushResourceObject(Icons[5]);
		Texts[Index]->SetText(NSLOCTEXT("UI", "Crafting", "제조하기"));
		{
			FScriptDelegate Delegate;
			Delegate.BindUFunction(InventoryActor, FName("OpenCraft"));
			Buttons[Index]->OnClicked.Clear();
			Buttons[Index]->OnClicked.Add(Delegate);
		}
		break;
	case UBInventoryWidgetBehavior::Use:
		Images[Index]->SetBrushResourceObject(Icons[6]);
		Texts[Index]->SetText(NSLOCTEXT("UI", "Use", "사용하기"));
		{
			FScriptDelegate Delegate;
			Delegate.BindUFunction(InventoryActor, FName("ItemUse"));
			Buttons[Index]->OnClicked.Clear();
			Buttons[Index]->OnClicked.Add(Delegate);
		}
		break;
	default:
		break;
	}

	Panels[Index]->SetVisibility(ESlateVisibility::Visible);
}

void UBInventoryWidgetBehavior::ButtonTest()
{
	LOG_MSG(TEXT("Button Click"));
}

void UBInventoryWidgetBehavior::SetCraft()
{
	LOG_MSG(TEXT("Craft Click"));
}