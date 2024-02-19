// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "Actor/Generic/Interface/BInteraction.h"
#include "BGeometryInteractionActor.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABGeometryInteractionActor : public AGeometryCollectionActor, public IBInteraction
{
	GENERATED_BODY()
	
public:
	// ���� ��ȣ�ۿ��� ������ ��������
	virtual bool AbleInteraction() const override;

	// ���� ��ȣ�ۿ��� ���¸� ��ȯ
	virtual EInteraction GetInteractionType() const override;
};
