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
	
	// HUD의 최대 체력을 지정합니다. 0.37이 초기 최대체력이며 0.75가 게임 내 최대 HP상태 입니다
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetMaxHP(float Percent);

	// HUD의 체력을 지정합니다. 0은 체력이 없는 상태, 1은 체력이 가득 찬 상태입니다
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHP(float Percent);

	// HUD의 대미지를 지정합니다. 대미지 이전 체력과 현재 체력을 입력해주세요
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHPDamage(float BeforePercent, float CurPercent);

	// ________________Gun____________________________

	// HUD의 무기를 지정합니다. Empty는 무장해제 상태입니다. 총 및 Empty 외의 아이템코드는 Warning 로그를 표시합니다.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetWeapon(EItemCode Weapon);

	// 무기의 현재 장전된 탄 수를 지정합니다.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetLoadAmmo(int32 Ammo);

	// 무기의 여분 탄 수를 지정합니다.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetStoredAmmo(int32 Ammo);

	// ________________Knife________________________

	// HUD의 나이프 표시 여부를 지정합니다. true 시 나이프 아이콘이 표시됩니다.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetKnife(bool IsVisible);

	// 나이프의 내구도를 지정합니다. 0은 파괴된 상태, 1은 온전한 상태입니다.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetKnifeDurability(float Durability);

	// ________________HotKey_______________________

	// HUD의 단축키 표시 여부를 지정합니다. true 시 핫키를 표시합니다.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHotKeyVisible(bool IsVisible);

	// 단축키를 눌렀을때 HUD에서 이를 표현합니다.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHotKeyNumber(int Num);
};
