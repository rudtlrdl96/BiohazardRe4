// Fill out your copyright notice in the Description page of Project Settings.


#include "MapUI/BMapUIActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ABMapUIActor::ABMapUIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//��Ʈ ������Ʈ
	RootPivotComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageMapRootComponent"));
	RootComponent = RootPivotComponent;

	// �� �޽�
	StageMapMesh.SetNum(4);
	StageMapMesh[1] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageMap_1F"));
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/UI/MapUI/Mesh/ui_village_1F_MeshMerge.ui_village_1F_MeshMerge'"));
		StageMapMesh[1]->SetStaticMesh(Mesh.Object);
	}
	StageMapMesh[1]->SetupAttachment(RootComponent);
	StageMapMesh[1]->SetVisibility(false);

	StageMapMesh[2] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageMap_2F"));
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/UI/MapUI/Mesh/ui_village_2F_MeshMerge.ui_village_2F_MeshMerge'"));
		StageMapMesh[2]->SetStaticMesh(Mesh.Object);
	}
	StageMapMesh[2]->SetupAttachment(RootComponent);
	StageMapMesh[2]->SetVisibility(false);

	StageMapMesh[3] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageMap_3F"));
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/UI/MapUI/Mesh/ui_village_3F_MeshMerge.ui_village_3F_MeshMerge'"));
		StageMapMesh[3]->SetStaticMesh(Mesh.Object);
	}
	StageMapMesh[3]->SetupAttachment(RootComponent);
	StageMapMesh[3]->SetVisibility(false);

	StageMapMesh[0] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageMap_B1"));
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/UI/MapUI/Mesh/ui_village_B1_MeshMerge.ui_village_B1_MeshMerge'"));
		StageMapMesh[0]->SetStaticMesh(Mesh.Object);
	}
	StageMapMesh[0]->SetupAttachment(RootComponent);
	StageMapMesh[0]->SetVisibility(false);

	SetFloor(EFloor::E_1F); // �������� �� �ʱ�ȭ

	// ī�޶�
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation({ 0, 65, 300 });
	Camera->SetRelativeRotation({ -80, -90, 0 });
	Camera->FieldOfView = 25.0f;

}

void ABMapUIActor::SetFloor(EFloor Floor)
{
	StageMapMesh[CurrentFloor]->SetVisibility(false);
	CurrentFloor = static_cast<uint8>(Floor);
	StageMapMesh[CurrentFloor]->SetVisibility(true);
}


// Called when the game starts or when spawned
void ABMapUIActor::BeginPlay()
{
	Super::BeginPlay();
	bMapUIOnOffSwitch = false;

	StageMapMesh[CurrentFloor]->SetHiddenInGame(!bMapUIOnOffSwitch);
	SetHidden(!bMapUIOnOffSwitch);
	SetActorTickEnabled(bMapUIOnOffSwitch);
}

// Called every frame
void ABMapUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABMapUIActor::MapUIOn()
{
	bMapUIOnOffSwitch = true;
	UGameplayStatics::GetPlayerController(this, 0)->SetViewTarget(this);
	
	StageMapMesh[CurrentFloor]->SetHiddenInGame(!bMapUIOnOffSwitch);
	SetHidden(!bMapUIOnOffSwitch);
	SetActorTickEnabled(bMapUIOnOffSwitch);
}

void ABMapUIActor::MapUIOff()
{
	bMapUIOnOffSwitch = false;
	UGameplayStatics::GetPlayerController(this, 0)->SetViewTarget(UGameplayStatics::GetPlayerPawn(this, 0));
	
	StageMapMesh[CurrentFloor]->SetHiddenInGame(!bMapUIOnOffSwitch);
	SetHidden(!bMapUIOnOffSwitch);
	SetActorTickEnabled(bMapUIOnOffSwitch);
}
