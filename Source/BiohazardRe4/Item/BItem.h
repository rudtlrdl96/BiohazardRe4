// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/Generic/Interface/BInteraction.h"
#include "ItemData.h"
#include "BItem.generated.h"

UCLASS()
class BIOHAZARDRE4_API ABItem : public AActor, public IBInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ���� ��ȣ�ۿ��� ������ ��������
	virtual bool AbleInteraction() const { return !bIsPickup; }
	// ���� ��ȣ�ۿ��� ���¸� ��ȯ
	virtual EInteraction GetInteractionType() const { return EInteraction::DropItem; }
	// UI Pivot ��ġ�� ��ȯ
	virtual FVector GetUIPivot() const { return FVector::ZeroVector; }
	// �������� �ݴ´�
	void PickUp();
	UFUNCTION(BlueprintImplementableEvent)
	void PickUpEvent();

	UPROPERTY(EditAnywhere ,BlueprintReadWrite, Category="Item")
	EItemCode ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Count;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
	uint32 bCanPickup : 1;
	UPROPERTY(BlueprintReadWrite, Category = "Item")
	uint32 bIsPickup : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	class USphereComponent* Collision;
};
