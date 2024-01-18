// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryActor.h"
#include "InventoryManager.h"
#include "InventorySlot.h"
#include "InventoryItem.h"
#include "InventoryWidget.h"
#include "Generic/BFsm.h"

#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ABInventoryActor::ABInventoryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 인벤토리 가방 메쉬
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

	// 카메라
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation({ 0, 25, 70 });
	Camera->SetRelativeRotation({ -80, -90, 0 });
	
	// 배경
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

	// 인벤토리 관리자
	Inventory = CreateDefaultSubobject<UBInventoryManager>(TEXT("Inventory Manager"));
	Inventory->SetupAttachment(CaseMesh);

	// 인벤토리 슬롯
	auto& Slot = Inventory->Slot;
	Slot.SetNum(10 * 7);
	for (int i = 0; i < 10 * 7; i++)
	{
		Slot[i] = CreateDefaultSubobject<UBInventorySlot>(FName(TEXT("Slot") + FString::FromInt(i)));
		Slot[i]->SetupAttachment(Inventory);
		Slot[i]->SetBoxExtent({ 2.5, 2.5, 0.1 });
		Slot[i]->SetCollisionProfileName(TEXT("UI"));
		int x = i % 10;
		int y = i / 10;
		Slot[i]->SetLocation({ x, y });
		Slot[i]->SetRelativeLocation({ -22.5 + 5 * x, -12.5 + 5 * y, 0 });
	}

	// ______ FSM
	FSMComp = CreateDefaultSubobject<UBFsm>(TEXT("FSM"));

	UBFsm::FStateCallback DefaultState;
	DefaultState.EnterDel.BindUObject(this,	&ABInventoryActor::DefaultEnter);
	DefaultState.UpdateDel.BindUObject(this, &ABInventoryActor::DefaultUpdate);
	DefaultState.ExitDel.BindUObject(this, &ABInventoryActor::DefaultExit);
	UBFsm::FStateCallback DragState;
	DragState.EnterDel.BindUObject(this, &ABInventoryActor::DragEnter);
	DragState.UpdateDel.BindUObject(this, &ABInventoryActor::DragUpdate);
	DragState.ExitDel.BindUObject(this, &ABInventoryActor::DragExit);

	FSMComp->CreateState(TO_KEY(EInventoryState::Default), DefaultState);
	FSMComp->CreateState(TO_KEY(EInventoryState::Drag), DragState);
	FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
}

// Called when the game starts or when spawned
void ABInventoryActor::BeginPlay()
{
	Super::BeginPlay();

	// 0번 PlayerController를 받아온다
	// UEnhancedInputLocalPlayerSubsystem을 받아온다
	// Subsystem에 MappingContext를 추가한다 (우선순위를 1번으로 두어서 0번으로 Mapping한 조작을 무시한다)
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Controller->GetLocalPlayer());
	Subsystem->AddMappingContext(DefaultMappingContext, 1);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(Controller->InputComponent);
	Input->BindAction(SelectAction, ETriggerEvent::Triggered, this, &ABInventoryActor::Click);
	Input->BindAction(DragAction, ETriggerEvent::Started, this, &ABInventoryActor::DragStart);
	Input->BindAction(DragAction, ETriggerEvent::Triggered, this, &ABInventoryActor::DragTrigger);
	Input->BindAction(DragAction, ETriggerEvent::Canceled, this, &ABInventoryActor::DragCancel);
	Input->BindAction(DebugAction, ETriggerEvent::Triggered, this, &ABInventoryActor::DebugAddPistol);

	Widget = CreateWidget<UBInventoryWidget>(GetWorld(), InventoryWidgetClass);
	Widget->AddToViewport();
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

void ABInventoryActor::Click()
{
	if (FSMComp->GetCurrentFSMKey() == TO_KEY(EInventoryState::Drag))
	{
		FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
	}
}

void ABInventoryActor::DragStart()
{
	if (SelectItem && FSMComp->GetCurrentFSMKey() == TO_KEY(EInventoryState::Default))
	{
		FSMComp->ChangeState(TO_KEY(EInventoryState::Drag));
	}
}

void ABInventoryActor::DragTrigger()
{
	bIsDragMove = 1;
}

void ABInventoryActor::DragCancel()
{
	if (FSMComp->GetCurrentFSMKey() == TO_KEY(EInventoryState::Drag))
	{
		FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
	}
}

void ABInventoryActor::DefaultEnter()
{
}

void ABInventoryActor::DefaultUpdate(float _DeltaTime)
{
	FHitResult Res;
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	FVector MousePos, MouseRot, End;
	Controller->DeprojectMousePositionToWorld(MousePos, MouseRot);
	End = MousePos + MouseRot * 1000;
	TEnumAsByte<EObjectTypeQuery> ObjectToTrace = EObjectTypeQuery::ObjectTypeQuery1;
	FCollisionObjectQueryParams Params = FCollisionObjectQueryParams();
	Params.AddObjectTypesToQuery(ECC_GameTraceChannel1);

	if (GetWorld()->LineTraceSingleByObjectType(Res, MousePos, End, Params))
	{
		UBInventorySlot* Slot = Cast<UBInventorySlot>(Res.Component);
		if (Slot && Slot != SelectSlot)
		{
			SelectSlot = Slot;
			if (UBInventoryItem* Item = Slot->GetItem())
			{
				SelectItem = Item;
				Widget->ItemName = Item->Data.ItemName.ToString();
			}
			else
			{
				SelectItem = nullptr;
				Widget->ItemName = TEXT("");
			}
		}
	}
	else if (SelectSlot)
	{
		SelectSlot = nullptr;
		SelectItem = nullptr;
		Widget->ItemName = TEXT("");
	}

}

void ABInventoryActor::DefaultExit()
{
}

void ABInventoryActor::DragEnter()
{
	if (!SelectItem)
	{
		FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
	}
	bIsDragMove = 0;
}

void ABInventoryActor::DragUpdate(float _DeltaTime)
{
	FHitResult Res;
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	FVector MousePos, MouseRot, End;
	Controller->DeprojectMousePositionToWorld(MousePos, MouseRot);
	End = MousePos + MouseRot * 1000;
	TEnumAsByte<EObjectTypeQuery> ObjectToTrace = EObjectTypeQuery::ObjectTypeQuery1;
	FCollisionObjectQueryParams Params = FCollisionObjectQueryParams();
	Params.AddObjectTypesToQuery(ECC_GameTraceChannel1);

	if (GetWorld()->LineTraceSingleByObjectType(Res, MousePos, End, Params))
	{
		UBInventorySlot* Slot = Cast<UBInventorySlot>(Res.Component);
		if (Slot)
		{
			SelectSlot = Slot;
			// 일단 이동시켜놓고 있음.
			// 다른 아이템을 덮어씌우는 문제 있음
			if (bIsDragMove == 1)
			{
				Inventory->MoveItem(SelectItem, Slot->GetLocation());
			}
		}
	}
}

void ABInventoryActor::DragExit()
{
	if (SelectItem && bIsDragMove)
	{
		Inventory->MoveItemConfirm(SelectItem, SelectSlot->GetLocation());
		SelectSlot = nullptr;
		SelectItem = nullptr;
	}
	bIsDragMove = 0;
}
