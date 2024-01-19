// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryBehavior.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"

void UBInventoryBehavior::NativeOnInitialized()
{
	Button = Cast<UButton>(GetWidgetFromName(TEXT("Button_0")));
	SetPositionInViewport({ 500, 500 });
}

void UBInventoryBehavior::SetButton()
{
}
