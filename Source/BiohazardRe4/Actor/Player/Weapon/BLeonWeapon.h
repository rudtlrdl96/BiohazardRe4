// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Engine/DamageEvents.h"
#include "DamageType/BDMGPlayerDamage.h"
#include "BiohazardRe4.h"
#include "..\Body\BLeon.h"
#include "BLeonWeapon.generated.h"

class UProjectileMovementComponent;

UCLASS()
class BIOHAZARDRE4_API ABLeonWeapon : public AActor
{
	GENERATED_BODY()
	
protected:
	class ABInventoryActor* InventoryInst;

public:
	// Sets default values for this actor's properties
	ABLeonWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPlayer(ABLeon* _PlayerPtr);

protected:
	// Called when the game starts or when spawned	
	virtual void BeginPlay() override;

public:
	
	// 공격 가능 여부 반환 함수, 장탄수 반환 또는 칼 내구도 반환
	virtual bool AbleAttack() const;

	// 공격 실행 함수
	virtual void Attack();

	// 재장전 실행 가능 반환 함수, 칼 또는 수류탄의 경우 불필요
	virtual bool AbleReload() const;

	// 재장전 함수, 칼 또는 수류탄의 경우 불필요
	virtual void Reload();

	// 재장전 함수, 칼 또는 수류탄의 경우 불필요
	virtual void ReloadStart();

	// 남은 탄약 수 반환, 칼의 경우 내구도를 반환
	virtual uint32 GetAmmo() const;

	FTransform GetLeftHandSocketTransform() const;

	void ThrowWeapon(const FVector& _Velocity);

	void ActiveSimulatePhysics(bool _bIsActive);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Mesh")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	ABLeon* Player = nullptr;

	UPROPERTY()
	FDamageEvent WeaponDamageEvent;

	//WeaponInformation
	UPROPERTY()
	UBDMGPlayerDamage* WeaponType;

	UPROPERTY()
	int32 WeaponDamage;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	UProjectileMovementComponent* ProjectileComp = nullptr;
};