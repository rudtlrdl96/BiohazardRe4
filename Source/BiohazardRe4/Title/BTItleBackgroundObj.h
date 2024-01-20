// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BTItleBackgroundObj.generated.h"

UCLASS()
class BIOHAZARDRE4_API ABTItleBackgroundObj : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABTItleBackgroundObj();

	double GetRatio() const
	{
		double WholeLen = FVector::Dist(StartLocation, EndLocation);
		double CurLen = FVector::Dist(GetActorLocation(), EndLocation);
		return CurLen / WholeLen;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	FVector StartLocation = { 5000.f,0.f,0.f };
	UPROPERTY(EditAnywhere)
	FVector EndLocation = FVector::Zero();
	UPROPERTY(EditAnywhere)
	float Velocity = 1000.f;

};
