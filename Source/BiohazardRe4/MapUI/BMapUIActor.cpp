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

	//루트 컴포넌트
	RootPivotComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageMapRootComponent"));
	RootComponent = RootPivotComponent;

	// 맵 메쉬
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

	SetFloor(EFloor::E_1F); // 보여지는 층 초기화

	// 카메라
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation({ 0, 65, 300 });
	Camera->SetRelativeRotation({ -90, -90, 0 });
	Camera->FieldOfView = 45.0f;

	//배경 메쉬
	BackgroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MapUIBackground"));
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/UI/MapUI/Mesh/Backgrouond_Plane.Backgrouond_Plane'"));
		BackgroundMesh->SetStaticMesh(Mesh.Object);
	}
	FVector InitLocation = FVector(100.f,-40.f,0);
	FVector InitScale = FVector(8.f,6.25,1);
	BackgroundMesh->SetRelativeLocation(InitLocation);
	BackgroundMesh->SetRelativeScale3D(InitScale);
}

void ABMapUIActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent)
{
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
	BackgroundMesh->SetHiddenInGame(!bMapUIOnOffSwitch);
	SetHidden(!bMapUIOnOffSwitch);
	SetActorTickEnabled(bMapUIOnOffSwitch);

	Controller = UGameplayStatics::GetPlayerController(this, 0);
	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Controller->GetLocalPlayer());

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(Controller->InputComponent);
	Input->BindAction(CameraDragStart, ETriggerEvent::Started, this, &ABMapUIActor::CameraDragStartFunc);
	Input->BindAction(CameraMove, ETriggerEvent::Triggered, this, &ABMapUIActor::CameraMoveFunc);
	Input->BindAction(CameraDragEnd, ETriggerEvent::Completed, this, &ABMapUIActor::CameraDragEndFunc);
	Input->BindAction(CameraZoom, ETriggerEvent::Triggered, this, &ABMapUIActor::CameraZoomFunc);
	Input->BindAction(ViewUpperFloor, ETriggerEvent::Triggered, this, &ABMapUIActor::ViewUpperFloorFunc);
	Input->BindAction(ViewLowerFloor, ETriggerEvent::Triggered, this, &ABMapUIActor::ViewLowerFloorFunc);
	Input->BindAction(MapUIClose, ETriggerEvent::Triggered, this, &ABMapUIActor::MapUIOff);
}

// Called every frame
void ABMapUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABMapUIActor::MapUIOn()
{
	if (Controller == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MapUI Controller is nullptr"))
	}

	bMapUIOnOffSwitch = true;
	Controller->SetViewTarget(this);
	Subsystem->AddMappingContext(DefaultMappingContext, 1);

	StageMapMesh[CurrentFloor]->SetHiddenInGame(!bMapUIOnOffSwitch);
	BackgroundMesh->SetHiddenInGame(!bMapUIOnOffSwitch);
	SetHidden(!bMapUIOnOffSwitch);
	SetActorTickEnabled(bMapUIOnOffSwitch);
	Controller->SetShowMouseCursor(bMapUIOnOffSwitch);
}

void ABMapUIActor::MapUIOff()
{
	if (Controller == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MapUI Controller is nullptr"))
	}

	bMapUIOnOffSwitch = false;
	Controller->SetViewTarget(UGameplayStatics::GetPlayerPawn(this, 0));
	Subsystem->RemoveMappingContext(DefaultMappingContext);

	StageMapMesh[CurrentFloor]->SetHiddenInGame(!bMapUIOnOffSwitch);
	BackgroundMesh->SetHiddenInGame(!bMapUIOnOffSwitch);
	SetHidden(!bMapUIOnOffSwitch);
	SetActorTickEnabled(bMapUIOnOffSwitch);
	Controller->SetShowMouseCursor(bMapUIOnOffSwitch);
	Camera->FieldOfView = 45.0f;
}

void ABMapUIActor::CameraMoveFunc(const FInputActionValue& Value)
{

	if (!bCameraDrageState)
	{
		return;
	}
	
	FVector NextLoc = Camera->GetRelativeLocation() + (Value.Get<FVector>() * CameraMoveVelocity);

	if (NextLoc.X < CameraMovableRange.X)
	{
		NextLoc.X = CameraMovableRange.X;
	}

	if (NextLoc.Y < CameraMovableRange.Y)
	{
		NextLoc.Y = CameraMovableRange.Y;
	}
		
	if (NextLoc.X > CameraMovableRange.Z)
	{
		NextLoc.X = CameraMovableRange.Z;
	}
		
	if (NextLoc.Y > CameraMovableRange.W)
	{
		NextLoc.Y = CameraMovableRange.W;
	}
	
	Camera->SetRelativeLocation(NextLoc);
}

bool ABMapUIActor::IsOverCameraMoveRange(FVector3d CameraPos)
{
	return CameraPos.X < CameraMovableRange.X || CameraPos.Y < CameraMovableRange.Y || CameraPos.X > CameraMovableRange.Z || CameraPos.Y >CameraMovableRange.W;
}

void ABMapUIActor::CameraDragStartFunc()
{
	bCameraDrageState = true;
	Controller->SetShowMouseCursor(false);
}

void ABMapUIActor::CameraDragEndFunc()
{
	bCameraDrageState = false;
	Controller->SetShowMouseCursor(true);
}

void ABMapUIActor::ViewUpperFloorFunc()
{
	uint8 UpperFloor = CurrentFloor + 1;
	if (UpperFloor == static_cast<uint8>(EFloor::E_MAX))
	{
		UpperFloor = 0;
	}
	SetFloor(static_cast<EFloor>(UpperFloor));
}

void ABMapUIActor::ViewLowerFloorFunc()
{
	uint8 LowerFloor = CurrentFloor - 1;
	if (LowerFloor == UINT8_MAX)
	{
		LowerFloor = static_cast<uint8>(EFloor::E_3F);
	}
	SetFloor(static_cast<EFloor>(LowerFloor));
}

void ABMapUIActor::CameraZoomFunc(const struct FInputActionValue& Value)
{
	float MovementVector = Value.Get<float>() * 5 + Camera->FieldOfView;
	if (IsOverFOVRange(MovementVector))
	{
		return;
	}
	Camera->FieldOfView = MovementVector;
}



