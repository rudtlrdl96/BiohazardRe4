// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryActor.h"
#include "InventoryManager.h"
#include "InventoryItem.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ABInventoryActor::ABInventoryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	BackgroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	RootComponent = BackgroundMesh;
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
		BackgroundMesh->SetStaticMesh(Mesh.Object);

		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Game/UI/Inventory/M_Inventory.M_Inventory'"));
		BackgroundMesh->SetMaterial(0, Material.Object);

	}

	CaseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Case"));
	CaseMesh->SetupAttachment(RootComponent);
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/UI/Inventory/Mesh/sm77_600.sm77_600'"));
		CaseMesh->SetSkeletalMesh(Mesh.Object);

		static ConstructorHelpers::FObjectFinder<UMaterial> Material0(TEXT("/Script/Engine.Material'/Game/UI/Inventory/Mesh/Alumi_Mat.Alumi_Mat'"));
		CaseMesh->SetMaterial(0, Material0.Object);

		static ConstructorHelpers::FObjectFinder<UMaterial> Material1(TEXT("/Script/Engine.Material'/Game/UI/Inventory/Mesh/Nuno_Mat.Nuno_Mat'"));
		CaseMesh->SetMaterial(1, Material1.Object);
	}
	CaseMesh->SetRelativeLocation({ 0, -3.0, 30 });
	CaseMesh->SetRelativeScale3D({ 0.3, 0.3, 0.3 });

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation({ 0, 0, 50 });
	Camera->SetRelativeRotation({ -90, -90, 0 });

	Inventory = CreateDefaultSubobject<UBInventoryManager>(TEXT("Inventory Manager"));
	Inventory->SetupAttachment(CaseMesh);
}

// Called when the game starts or when spawned
void ABInventoryActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABInventoryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABInventoryActor::AddItem(const FName& _Name)
{
	Inventory->AddItem(_Name);
}