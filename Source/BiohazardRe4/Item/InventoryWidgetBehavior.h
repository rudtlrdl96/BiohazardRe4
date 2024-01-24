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

private:
	enum State_BehaviorButton
	{
		None,
		Equip,			// ���
		Unequip,		// �������
		Investigate,	// ����
		Hotkey,			// ����Ű ���
		Drop,			// ������
		Crafting,		// �����ϱ�
		Use,			// ����ϱ�
	};

	void SetButton(int Index, State_BehaviorButton State);

	UFUNCTION()
	void ButtonTest();

	UFUNCTION()
	void SetCraft();

};
