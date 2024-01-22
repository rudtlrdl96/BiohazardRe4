// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BMapUIActor.generated.h"

UENUM()
enum class EFloor : uint8
{
	TE_B1 UMETA(DisplayName = "B1"),
	TE_1F UMETA(DisplayName = "1F"),
	TE_2F UMETA(DisplayName = "2F"),
	TE_3F UMETA(DisplayName = "3F"),
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// __________________Mesh
	UPROPERTY()
	UStaticMeshComponent* RootPivotComponent;

	UPROPERTY()
	UStaticMeshComponent* BackgroundMesh;	// 배경 매쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UStaticMeshComponent*> StageMapMesh;	// 지도 메쉬(층별로 분류), 해당 스테이지가 몇층으로 이루어져있는지에 대한 정보 필요

	uint8 CurrentFloor = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UCameraComponent* Camera;

	// __________________Input

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* DefaultMappingContext = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	class UEnhancedInputLocalPlayerSubsystem* Subsystem;	// EnhancedSubSystem


};
