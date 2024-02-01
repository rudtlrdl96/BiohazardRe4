// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "BiohazardRe4.h"
#include "..\Body\BLeon.h"
#include "BLeonWeapon.generated.h"

UCLASS()
class BIOHAZARDRE4_API ABLeonWeapon : public AActor
{
	GENERATED_BODY()
	
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

	// ���� ź�� �� ��ȯ, Į�� ��� �������� ��ȯ
	virtual uint32 GetAmmo() const;

	FTransform GetLeftHandSocketTransform() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Mesh")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	ABLeon* Player = nullptr;

};