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
	
	// ���� ���� ���� ��ȯ �Լ�, ��ź�� ��ȯ �Ǵ� Į ������ ��ȯ
	virtual bool AbleAttack() const;

	// ���� ���� �Լ�
	virtual void Attack();

	// ������ ���� ���� ��ȯ �Լ�, Į �Ǵ� ����ź�� ��� ���ʿ�
	virtual bool AbleReload() const;

	// ������ �Լ�, Į �Ǵ� ����ź�� ��� ���ʿ�
	virtual void Reload();

	// ������ �Լ�, Į �Ǵ� ����ź�� ��� ���ʿ�
	virtual void ReloadStart();

	// ���� ź�� �� ��ȯ, Į�� ��� �������� ��ȯ
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