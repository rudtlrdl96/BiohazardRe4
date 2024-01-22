// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryBehavior.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UBInventoryBehavior::NativeOnInitialized()
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

void UBInventoryBehavior::SetItemData(const FBItemData& Data)
{
	for (UCanvasPanel* Panel : Panels)
	{
		Panel->SetVisibility(ESlateVisibility::Visible);
	}
	// 공용 기능 = 조사, 버리기(보관고 보내기)
	switch (Data.ItemCode)
	{
	case EItemCode::Handgun_SR09R:
	case EItemCode::Shotgun_W870:
	case EItemCode::Rifle_SRM1903:
		Images[0]->SetBrushResourceObject(Icons[0]);
		Texts[0]->SetText(NSLOCTEXT("UI", "Equip", "장비하기"));
		Images[1]->SetBrushResourceObject(Icons[1]);
		Texts[1]->SetText(NSLOCTEXT("UI", "Investigate", "조사하기"));
		Images[2]->SetBrushResourceObject(Icons[2]);
		Texts[2]->SetText(NSLOCTEXT("UI", "Hotkey", "단축키 등록"));
		Images[3]->SetBrushResourceObject(Icons[3]);
		Texts[3]->SetText(NSLOCTEXT("UI", "Drop", "버리기"));
		Panels[2]->SetVisibility(ESlateVisibility::Visible);
		Panels[3]->SetVisibility(ESlateVisibility::Visible);
		break;
	case EItemCode::CombatKnife:
	case EItemCode::HandgunAmmo:
	case EItemCode::ShotgunShells:
	case EItemCode::RifleAmmo:
		Texts[0]->SetText(NSLOCTEXT("UI", "Investigate", "조사하기"));
		Images[0]->SetBrushResourceObject(Icons[1]);
		Texts[1]->SetText(NSLOCTEXT("UI", "Drop", "버리기"));
		Images[1]->SetBrushResourceObject(Icons[3]);
		Panels[2]->SetVisibility(ESlateVisibility::Hidden);
		Panels[3]->SetVisibility(ESlateVisibility::Hidden);
		// 조사하기, 보관고(버리기)
		break;
	case EItemCode::GreenHerb:
		// 사용하기, 조사하기, 제조하기, 보관고(버리기)
		break;
	case EItemCode::MixedHerb_GG:
	case EItemCode::MixedHerb_GR:
	case EItemCode::FirstAidSpray:
		// 사용하기	
		break;
	case EItemCode::Gunpowder:
	case EItemCode::RedHerb:
	case EItemCode::Resources_S:
	case EItemCode::Resources_L:
		// 제조하기
		break;
	default:
		break;
	}
}

void UBInventoryBehavior::SetHide()
{
	for (UCanvasPanel* Panel : Panels)
	{
		Panel->SetVisibility(ESlateVisibility::Hidden);
	}
}
