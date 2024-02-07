// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Generic/BCollisionObserver.h"
#include "BCollisionObserverSphere.generated.h"

class USphereComponent;

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABCollisionObserverSphere : public ABCollisionObserver
{
	GENERATED_BODY()
	
public:
	ABCollisionObserverSphere();

	void SetRadius(float _Radius);
protected:
	virtual void CreateCollision() override;
private:
	USphereComponent* SphereCol = nullptr;
};
