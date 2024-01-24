// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BMapUIActor.generated.h"

UENUM()
enum class EFloor : uint8
{
	E_B1 UMETA(DisplayName = "B1"),
	E_1F UMETA(DisplayName = "1F"),
	E_2F UMETA(DisplayName = "2F"),
	E_3F UMETA(DisplayName = "3F"),
};

UCLASS()
class BIOHAZARDRE4_API ABMapUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABMapUIActor();

	UFUNCTION(BlueprintCallable)
	void SetFloor(EFloor Floor);

	UFUNCTION(BlueprintCallable)
	void MapUIOn();

	UFUNCTION(BlueprintCallable)
	void MapUIOff();


	UFUNCTION(BlueprintCallable)
	void MapUISwitch()
	{
		if (bMapUIOnOffSwitch)
		{
			MapUIOff();
		}
		else
		{
			MapUIOn();
		}
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// __________________Component
	UPROPERTY()
	UStaticMeshComponent* RootPivotComponent;

	UPROPERTY()
	UStaticMeshComponent* BackgroundMesh;	// 배경 매쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TArray<UStaticMeshComponent*> StageMapMesh;

	uint8 CurrentFloor = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UCameraComponent* Camera;

private:
	UPROPERTY()
	bool bMapUIOnOffSwitch = false;

	// __________________Input

	UPROPERTY(VisibleAnywhere, Category = Input)
	APlayerController* Controller = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* DefaultMappingContext = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* CameraDragStart = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* CameraMove = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* CameraDragEnd = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* ViewUpperFloor = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* ViewLowerFloor = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UEnhancedInputLocalPlayerSubsystem* Subsystem;	// EnhancedSubSystem

	bool bCameraDrageState = false;
	
	void CameraDragStartFunc();
	void CameraMoveFunc(const struct FInputActionValue& Value);
	void CameraDragEndFunc();
	void ViewUpperFloorFunc();
	void ViewLowerFloorFunc();
};
