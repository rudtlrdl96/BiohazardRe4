// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Item/ItemData.h"
#include "Actor/Generic/Interface/BInteraction.h"
#include "HUDBase.generated.h"

UCLASS()
class BIOHAZARDRE4_API ABHUDBase : public AHUD
{
	GENERATED_BODY()
	
public :

	ABHUDBase();

	static ABHUDBase* Instance;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateLoadedAmmo();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateStoredAmmo();

	// _________________ HP ______________________
	
	// HUD�� �ִ� ü���� �����մϴ�. 0.37�� �ʱ� �ִ�ü���̸� 0.75�� ���� �� �ִ� HP���� �Դϴ�
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetMaxHP(float Percent);

	// HUD�� ü���� �����մϴ�. 0�� ü���� ���� ����, 1�� ü���� ���� �� �����Դϴ�
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHP(float Percent);

	// ȸ�� ������ ����� �� ȸ���� ü���� �̸� �����ݴϴ�. ȸ�� �������� ����� ��� ��� �� ü�� �ۼ�Ʈ�� �Է����ּ���.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHealPreview(float Percent);

	// ȸ�� ������ �����並 ���ϴ�.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OffHealPreview();

	// ________________Gun____________________________
	
	// HUD�� ���⸦ �����մϴ�.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetWeapon(class ABInventoryWeapon* Weapon);

	// HUD�� ���⸦ �����մϴ�.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetWeaponCode(EItemCode Code);

	UFUNCTION(BlueprintCallable)
	ABInventoryWeapon* GetWeapon(EItemCode Code);

	// HUD�� ���⸦ �������� ���·� �����մϴ�.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ClearWeapon();
	// ������ ���� ������ ź ���� �����մϴ�.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetLoadAmmo(int32 Ammo);

	// ������ ���� ź ���� �����մϴ�.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetStoredAmmo(int32 Ammo);

	// ________________Knife________________________

	// HUD�� ������ ǥ�� ���θ� �����մϴ�. true �� ������ �������� ǥ�õ˴ϴ�.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetKnife(bool IsVisible);

	// �������� �������� �����մϴ�. 0�� �ı��� ����, 1�� ������ �����Դϴ�.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetKnifeDurability(float Durability);

	// ________________QuickSlot_______________________

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void QuickSlotUpdate(const TArray<ABInventoryWeapon*>& QuickSlot);

	// HUD�� ����Ű ǥ�� ���θ� �����մϴ�. true �� ��Ű�� ǥ���մϴ�.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetQuickSlotVisible(bool IsVisible);

	// ����Ű�� �������� HUD���� �̸� ǥ���մϴ�.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetQuickSlotNumber(int Num);

	// _______________Interaction_____________________
	// HUD�� ��ȣ�ۿ� Ű ǥ�ø� �����մϴ�
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetInteraction(EInteraction Intreaction);
	// HUD�� ��ȣ�ۿ� Ű ǥ�ø� ���ϴ�
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OffInteraction();

	// ______________Crosshair___________________

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetCrosshair(bool IsVisible);
};
