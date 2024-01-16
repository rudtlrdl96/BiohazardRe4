// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

// Sets default values for this component's properties
UBInventoryItem::UBInventoryItem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(this);
}


// Called when the game starts
void UBInventoryItem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBInventoryItem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBInventoryItem::SetItemData(const FBItemData& _Data)
{
	Data = _Data;
	Mesh->RegisterComponent();
	Mesh->SetStaticMesh(_Data.Mesh);
}

