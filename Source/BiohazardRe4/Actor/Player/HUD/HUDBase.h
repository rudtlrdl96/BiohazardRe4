// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Item/ItemData.h"
#include "HUDBase.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABHUDBase : public AHUD
{
	GENERATED_BODY()
	
public :

	// _________________ HP ______________________
	
	// HUD�� �ִ� ü���� �����մϴ�. 0.37�� �ʱ� �ִ�ü���̸� 0.75�� ���� �� �ִ� HP���� �Դϴ�
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetMaxHP(float Percent);

	// HUD�� ü���� �����մϴ�. 0�� ü���� ���� ����, 1�� ü���� ���� �� �����Դϴ�
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHP(float Percent);

	// HUD�� ������� �����մϴ�. ����� ���� ü�°� ���� ü���� �Է����ּ���
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHPDamage(float BeforePercent, float CurPercent);

	// ________________Gun____________________________

	// HUD�� ���⸦ �����մϴ�. Empty�� �������� �����Դϴ�. �� �� Empty ���� �������ڵ�� Warning �α׸� ǥ���մϴ�.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetWeapon(EItemCode Weapon);

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

	// ________________HotKey_______________________

	// HUD�� ����Ű ǥ�� ���θ� �����մϴ�. true �� ��Ű�� ǥ���մϴ�.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHotKeyVisible(bool IsVisible);

	// ����Ű�� �������� HUD���� �̸� ǥ���մϴ�.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHotKeyNumber(int Num);
};
