// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Map/BJumpObstacleTrigger.h"
#include "BJumpObstacleLineTrigger.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABJumpObstacleLineTrigger : public ABJumpObstacleTrigger
{
	GENERATED_BODY()
	
public:
	ABJumpObstacleLineTrigger();

	// ���� ���� ����� Ʈ���� ��ġ�� ��ȯ�մϴ�
	virtual FJumpData GetJumpMetaData(const FVector& _Location) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Jump")
	UBoxComponent* ObstacleWallTrigger = nullptr;
};
