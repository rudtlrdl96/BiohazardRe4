// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Generic/BCollisionObserver.h"
#include "BCollisionObserverCapsule.generated.h"

class UCapsuleComponent;

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABCollisionObserverCapsule : public ABCollisionObserver
{
	GENERATED_BODY()

public:
	ABCollisionObserverCapsule();

	void SetRadius(float _Radius);
	void SetHeight(float _Height);

protected:
	virtual void CreateCollision() override;

private:
	UCapsuleComponent* CapsuleCol = nullptr;
};
