// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "InventoryBehavior.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBInventoryBehavior : public UUserWidget
{
	GENERATED_BODY()

	friend class ABInventoryActor;

	virtual void NativeOnInitialized() override;

	UPROPERTY()
	ABInventoryActor* InventoryActor;

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
	void SetItemData(const FBItemData& Data);

	void SetHide();



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
