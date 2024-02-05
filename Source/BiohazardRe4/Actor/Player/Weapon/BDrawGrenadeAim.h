// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BDrawGrenadeAim.generated.h"

class USplineComponent;
class USplineMeshComponent;
struct FPredictProjectilePathResult;

UCLASS()
class BIOHAZARDRE4_API ABDrawGrenadeAim : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABDrawGrenadeAim();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Draw(const FPredictProjectilePathResult& _Path, bool _bIsSplineDraw);
	void DisableDraw();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	static const int32 MaxMeshCount;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* SplineComp = nullptr;

	TArray<USplineMeshComponent*> SplineMeshs;

	UStaticMeshComponent* DecalMesh = nullptr;
};
