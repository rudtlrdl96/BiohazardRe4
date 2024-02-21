// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BMapUIActor.generated.h"

UENUM(BluePrintType)
enum class EFloor : uint8
{
	E_B1 UMETA(DisplayName = "B1"),
	E_1F UMETA(DisplayName = "1F"),
	E_2F UMETA(DisplayName = "2F"),
	E_3F UMETA(DisplayName = "3F"),
	E_MAX UMETA(DisplayName = "MAX")
};

UCLASS()
class BIOHAZARDRE4_API ABMapUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABMapUIActor();

	static ABMapUIActor* MapUIInst;

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

	void SetCameraZoom(float FOV);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// __________________Base
	UPROPERTY()
	class ABLeon* MainPlayer;

	UPROPERTY()
	FVector StartLocation = {-10000, -10000, -10000};


	// __________________Component
	UPROPERTY()
	UStaticMeshComponent* RootPivotComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* BackgroundMesh;	// 배경 매쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TArray<UStaticMeshComponent*> StageMapMesh;

	uint8 CurrentFloor = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UBMapUIPlayerSprite* PlayerSprite;

	UPROPERTY()
	float MapScale = 0.01;

	// __________________Option
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
	float CameraMoveVelocity = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
	float CameraMaxFOV = 75.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
	float CameraMinFOV = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
	FVector4f CameraMovableRange = FVector4f::Zero();

	// __________________Widget
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UBMapUIWidgetMain> MapUIWidgetClass;

	UPROPERTY()
	UBMapUIWidgetMain* Widget;

	

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
	class UInputAction* CameraZoom = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* MapUIClose = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UEnhancedInputLocalPlayerSubsystem* Subsystem;	// EnhancedSubSystem

	bool bCameraDrageState = false;

	bool IsOverFOVRange(float CheckFOV)
	{
		return CheckFOV < CameraMinFOV || CheckFOV > CameraMaxFOV;
	}

	void CameraDragStartFunc();
	void CameraMoveFunc(const struct FInputActionValue& Value);
	void CameraDragEndFunc();
	void CameraZoomFunc(const struct FInputActionValue& Value);
	void ViewUpperFloorFunc();
	void ViewLowerFloorFunc();

	// __________________Sound

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* UIControlSound = nullptr;

	void PlayUIControlSound();
};
