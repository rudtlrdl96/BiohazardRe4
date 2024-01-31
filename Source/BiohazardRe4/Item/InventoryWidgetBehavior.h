// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "InventoryWidgetBehavior.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBInventoryWidgetBehavior : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeOnInitialized() override;

	UPROPERTY()
	TArray<class UCanvasPanel*> Panels;
	UPROPERTY()
	TArray<class UButton*> Buttons;
	UPROPERTY()
	TArray<class UImage*> Images;

	UPROPERTY()
	TArray<class UTextBlock*> Texts;

	UPROPERTY(EditAnywhere)
	TArray<UObject*> Icons;

public:
	class ABInventoryActor* InventoryActor;

	void SetItemData(const FBItemData& Data);

	UFUNCTION(BlueprintImplementableEvent)
	void WidgetOn();
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetOff();

private:
	enum State_BehaviorButton
	{
		None,
		Equip,			// 장비
		Unequip,		// 장비해제
		Investigate,	// 조사
		QuickSlot,			// 단축키 등록
		Drop,			// 버리기
		Crafting,		// 제조하기
		Use,			// 사용하기
	};

	void SetButton(int Index, State_BehaviorButton State);

};
