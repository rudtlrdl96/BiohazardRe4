// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryCursor.h"
#include "InventoryManager.h"

// Sets default values for this component's properties
UBInventoryCursor::UBInventoryCursor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UBInventoryCursor::SetCursorSize(const FIntPoint& Size)
{
	SetRelativeScale3D(FVector(Size.X, Size.Y, 1));
}

void UBInventoryCursor::SetCursorPosition(const FIntPoint& Pos)
{
	SetRelativeLocation(FVector(-25 + Pos.X * 5, -15 + Pos.Y * 5, 0));
}

void UBInventoryCursor::SetCursorRaise(bool Raise)
{
	FVector Location = CursorMesh->GetRelativeLocation();
	if (Raise)
	{
		Location.Z = 5;
	}
	else
	{
		Location.Z = 0;
	}
	CursorMesh->SetRelativeLocation(Location);
}


// Called when the game starts
void UBInventoryCursor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBInventoryCursor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

