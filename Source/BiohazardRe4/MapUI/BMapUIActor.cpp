// Fill out your copyright notice in the Description page of Project Settings.


#include "MapUI/BMapUIActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BMapUIWidgetMain.h"
#include "BMapUIPlayerSprite.h"

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
	RootComponent->SetWorldLocation(StartLocation);

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
	BackgroundMesh->SetupAttachment(RootComponent);
	FVector InitLocation = FVector(95.f,-41.f,0);
	FVector InitScale = FVector(8.5f,6.6f,1);
	BackgroundMesh->SetRelativeLocation(InitLocation);
	BackgroundMesh->SetRelativeScale3D(InitScale);
	FVector BoundMin;
	FVector BoundMax;
	BackgroundMesh->GetLocalBounds(BoundMin, BoundMax);
	BoundMin = BoundMin * InitScale + InitLocation;
	BoundMax = BoundMax * InitScale + InitLocation;
	CameraMovableRange = FVector4f(BoundMin.X, BoundMin.Y, BoundMax.X, BoundMax.Y);

	//플레이어 스프라이트
	PlayerSprite = CreateDefaultSubobject<UBMapUIPlayerSprite>(TEXT("MapUIPlayer"));
	{
		//static ConstructorHelpers::FObjectFinder<UPaperSprite> Sprite(TEXT("/Script/Paper2D.PaperSprite'/Game/Assets/UI/MapUI/Sprite/S_cs_ui3100_IAM_texout_Sprite_25.S_cs_ui3100_IAM_texout_Sprite_25'"));
		//Player->SetSprite(Sprite.Object);
	}
	PlayerSprite->SetupAttachment(RootComponent);
	PlayerSprite->SetRelativeLocation({ 0.f,0.f,2.f });
	PlayerSprite->SetRelativeScale3D({ 0.2f,0.2f,0.2f });
	PlayerSprite->SetVisibility(false);
}

void ABMapUIActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent)
{
}

void ABMapUIActor::SetFloor(EFloor Floor)
{
	Widget->SetCurrentFloor(Floor);
	Widget->SetPrevFloor(static_cast<EFloor>(CurrentFloor));
	StageMapMesh[CurrentFloor]->SetVisibility(false);
	CurrentFloor = static_cast<uint8>(Floor);
	StageMapMesh[CurrentFloor]->SetVisibility(true);
	Widget->UpdateMapLayerWidget();
}


void ABMapUIActor::SetCameraZoom(float FOV)
{
	Camera->FieldOfView = FOV;
	Widget->SetCameraFOV(FOV);
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

	//위젯
	Widget = CreateWidget<UBMapUIWidgetMain>(GetWorld(), MapUIWidgetClass);
	if (Widget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MapUIWidgetMain is nullptr"))
	}
	Widget->AddToViewport();
	Widget->SetVisibility(ESlateVisibility::Collapsed);

	SetFloor(EFloor::E_1F); // 보여지는 층 초기화
	SetCameraZoom(45.f);
	Widget->SetRangeOfFOV(CameraMinFOV, CameraMaxFOV);

	//플레이어 컴포넌트
	MainPlayer = UGameplayStatics::GetPlayerPawn(this, 0);
}

// Called every frame
void ABMapUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector PlayerLocation = MainPlayer->GetActorLocation() * MapScale;
	PlayerSprite->SetRelativeLocation({ PlayerLocation.X, PlayerLocation.Y, 2.f });
	
	FRotator PlayerRotation = MainPlayer->GetActorRotation();
	PlayerSprite->SetRelativeRotation({ 0.f, PlayerRotation.Yaw, 90.f});
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
	PlayerSprite->SetVisibility(bMapUIOnOffSwitch);
	SetHidden(!bMapUIOnOffSwitch);
	SetActorTickEnabled(bMapUIOnOffSwitch);
	Controller->SetShowMouseCursor(bMapUIOnOffSwitch);

	Widget->SetVisibility(ESlateVisibility::Visible);
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
	PlayerSprite->SetVisibility(bMapUIOnOffSwitch);
	SetHidden(!bMapUIOnOffSwitch);
	SetActorTickEnabled(bMapUIOnOffSwitch);
	Controller->SetShowMouseCursor(bMapUIOnOffSwitch);
	SetCameraZoom(45.f);

	Widget->SetVisibility(ESlateVisibility::Collapsed);
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
	SetCameraZoom(MovementVector);
}



