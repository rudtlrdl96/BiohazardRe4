// Fill out your copyright notice in the Description page of Project Settings.


#include "BItem.h"
#include "Components/SphereComponent.h"
#include "Item/InventoryActor.h"
// Sets default values
ABItem::ABItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ABItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABItem::PickUp()
{
	ABInventoryActor::Instance->AddItem(ItemCode, Count);
	PickUpEvent();
}

