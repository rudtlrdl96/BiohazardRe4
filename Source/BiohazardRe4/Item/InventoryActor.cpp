// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryActor.h"
#include "InventoryManager.h"
#include "InventoryItem.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ABInventoryActor::ABInventoryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	CaseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Case"));
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/UI/Inventory/Mesh/sm77_600.sm77_600'"));
		CaseMesh->SetSkeletalMesh(Mesh.Object);

		static ConstructorHelpers::FObjectFinder<UMaterial> Material0(TEXT("/Script/Engine.Material'/Game/UI/Inventory/Mesh/Alumi_Mat.Alumi_Mat'"));
		CaseMesh->SetMaterial(0, Material0.Object);

		static ConstructorHelpers::FObjectFinder<UMaterial> Material1(TEXT("/Script/Engine.Material'/Game/UI/Inventory/Mesh/Nuno_Mat.Nuno_Mat'"));
		CaseMesh->SetMaterial(1, Material1.Object);
	}
	RootComponent = CaseMesh;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation({ 0, 25, 70 });
	Camera->SetRelativeRotation({ -80, -90, 0 });

	BackgroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
		BackgroundMesh->SetStaticMesh(Mesh.Object);

		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Game/Assets/UI/Inventory/M_Inventory_Background.M_Inventory_Background'"));
		BackgroundMesh->SetMaterial(0, Material.Object);

	}
	BackgroundMesh->SetupAttachment(Camera);
	BackgroundMesh->SetRelativeLocation({ 150, 0, 0 });
	BackgroundMesh->SetRelativeRotation({ 0, 90, 90 });
	BackgroundMesh->SetRelativeScale3D({ 2.4, 2.4, 1.0 });

	Inventory = CreateDefaultSubobject<UBInventoryManager>(TEXT("Inventory Manager"));
	Inventory->SetupAttachment(CaseMesh);

	Boxs.SetNum(10 * 7);
	for (int i = 0; i < Boxs.Num(); i++)
	{
		Boxs[i] = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Box") + FString::FromInt(i)));
		Boxs[i]->SetupAttachment(Inventory);
		Boxs[i]->SetBoxExtent({ 2.5, 2.5, 5 });
		int x = i % 10;
		int y = i / 10;
		Boxs[i]->SetRelativeLocation({ -22.5 + 5 * x, -12.5 + 5 * y, 0 });
	}
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