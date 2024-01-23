// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryActor.h"
#include "InventoryManager.h"
#include "InventorySlot.h"
#include "InventoryItem.h"
#include "InventoryWidget.h"
#include "InventoryBehavior.h"
#include "InventoryCursor.h"
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
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/UI/Inventory/Mesh/SK_Case.SK_Case'"));
		CaseMesh->SetSkeletalMesh(Mesh.Object);

		static ConstructorHelpers::FObjectFinder<UAnimSequence> Anim(TEXT("/Script/Engine.AnimSequence'/Game/Assets/UI/Inventory/Animation/AS_CaseOpen.AS_CaseOpen'"));
		OpenAnim = Anim.Object;
	}
	RootComponent = CaseMesh;

	SubCaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubCase"));
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/UI/Inventory/Mesh/SM_SubCase.SM_SubCase'"));
		SubCaseMesh->SetStaticMesh(Mesh.Object);
	}
	SubCaseMesh->SetupAttachment(RootComponent);

	// 카메라
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation({ 0, 65, 300 });
	Camera->SetRelativeRotation({ -80, -90, 0 });
	Camera->FieldOfView = 25.0f;
	
	// 배경
	BackgroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
		BackgroundMesh->SetStaticMesh(Mesh.Object);

		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Game/Assets/UI/Inventory/M_Inventory_Background.M_Inventory_Background'"));
		BackgroundMesh->SetMaterial(0, Material.Object);
	}
	BackgroundMesh->SetupAttachment(Camera);
	BackgroundMesh->SetRelativeLocation({ 675.0f, 0, 0 });
	BackgroundMesh->SetRelativeRotation({ 0, 90, 90 });
	BackgroundMesh->SetRelativeScale3D({ 3.0, 3.0, 1.0 });

	// 인벤토리 관리자
	Inventory = CreateDefaultSubobject<UBInventoryManager>(TEXT("Inventory Manager"));
	Inventory->SetupAttachment(CaseMesh);

	// 인벤토리 슬롯
	auto& Slot = Inventory->MainSlot;
	Slot.SetNum(10 * 7);
	for (int i = 0; i < 10 * 7; i++)
	{
		Slot[i] = CreateDefaultSubobject<UBInventorySlot>(FName(TEXT("Slot") + FString::FromInt(i)));
		Slot[i]->SetupAttachment(Inventory);
		Slot[i]->SetBoxExtent({ 2.5, 2.5, 0.1 });
		Slot[i]->SetCollisionProfileName(TEXT("UI"));
		int x = i % 10;
		int y = i / 10;
		Slot[i]->SetPosition({ x, y });
		Slot[i]->SetRelativeLocation({ -22.5 + 5 * x, -12.5 + 5 * y, 0 });
	}

	auto& SubSlot = Inventory->SubSlot;
	SubSlot.SetNum(5 * 9);
	for (int i = 0; i < 5 * 9; i++)
	{
		SubSlot[i] = CreateDefaultSubobject<UBInventorySlot>(FName(TEXT("SubSlot") + FString::FromInt(i)));
		SubSlot[i]->SetupAttachment(SubCaseMesh);
		SubSlot[i]->SetBoxExtent({ 2.5, 2.5, 0.1 });
		SubSlot[i]->SetCollisionProfileName(TEXT("UI"));
		int x = i % 5;
		int y = i / 5;
		SubSlot[i]->SetPosition({ x, y });
		SubSlot[i]->SetRelativeLocation({ -10.0 + 5 * x, -20.0 + 5 * y, 2 });
		SubSlot[i]->SetSubSlot();

		SubSlot[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 커서
	Cursor = CreateDefaultSubobject<UBInventoryCursor>(TEXT("Cursor"));
	Cursor->SetupAttachment(RootComponent);
	// 커서메시
	Cursor->CursorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CursorMesh"));
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
		Cursor->CursorMesh->SetStaticMesh(Mesh.Object);

		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Engine/EditorResources/FieldNodes/_Resources/M_FieldRadiusPreview.M_FieldRadiusPreview'"));
		Cursor->CursorMesh->SetMaterial(0, Material.Object);
	}
	Cursor->CursorMesh->SetupAttachment(Cursor);
	Cursor->CursorMesh->SetRelativeLocation({ 2.5, 2.5, 2 });
	Cursor->CursorMesh->SetRelativeScale3D({ 0.05, 0.05, 0.05 });

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

	UBFsm::FStateCallback SelectState;

	FSMComp->CreateState(TO_KEY(EInventoryState::Default), DefaultState);
	FSMComp->CreateState(TO_KEY(EInventoryState::Drag), DragState);
	FSMComp->CreateState(TO_KEY(EInventoryState::Select), SelectState);
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
	//Subsystem->AddMappingContext(DefaultMappingContext, 1);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(Controller->InputComponent);
	Input->BindAction(SelectAction, ETriggerEvent::Triggered, this, &ABInventoryActor::Click);
	Input->BindAction(DragAction, ETriggerEvent::Started, this, &ABInventoryActor::DragStart);
	Input->BindAction(DragAction, ETriggerEvent::Triggered, this, &ABInventoryActor::DragTrigger);
	Input->BindAction(DragAction, ETriggerEvent::Canceled, this, &ABInventoryActor::DragCancel);
	Input->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ABInventoryActor::Turn);
	Input->BindAction(CancelAction, ETriggerEvent::Triggered, this, &ABInventoryActor::Cancel);

	Widget = CreateWidget<UBInventoryWidget>(GetWorld(), InventoryWidgetClass);
	Widget->AddToViewport();

	BehaviorWidget = CreateWidget<UBInventoryBehavior>(GetWorld(), BehaviorWidgetClass);
	BehaviorWidget->AddToViewport();
	BehaviorWidget->SetHide();

	FOnTimelineFloatStatic F;
	F.BindLambda([this](float Value) {
		SubCaseMesh->SetRelativeLocation(FMath::Lerp(FVector(100, 7.4, -2), FVector(45, 7.4, -2), Value));
		});
	Timeline.AddInterpFloat(CurveFloat, F);
}

// Called every frame
void ABInventoryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void ABInventoryActor::AddItem(const FName& _Name)
{
	Inventory->AddItem(_Name);
}

void ABInventoryActor::AddItem(EItemCode ItemCode)
{
	Inventory->AddItem(ItemCode);
}

void ABInventoryActor::OpenInventory()
{
	CaseMesh->PlayAnimation(OpenAnim, false);
	Subsystem->AddMappingContext(DefaultMappingContext, 1);
	UGameplayStatics::GetPlayerController(this, 0)->SetViewTarget(this);
}

void ABInventoryActor::OpenSub()
{
	Timeline.Play();
	auto& SubSlot = Inventory->SubSlot;
	for (int i = 0; i < 5 * 9; i++)
	{
		SubSlot[i]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ABInventoryActor::CloseSub()
{
	Timeline.Reverse();
	auto& SubSlot = Inventory->SubSlot;
	for (int i = 0; i < 5 * 9; i++)
	{
		SubSlot[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABInventoryActor::Click()
{
	if (bIsDragMove)
	{
		if (Inventory->IsEmptySlot(SelectSlot, SelectItem))
		{
			Inventory->MoveItemConfirm(SelectItem, SelectSlot);
			FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
		}
		else if (Inventory->CheckChange(SelectItem, SelectSlot))
		{
			SelectItem = Inventory->ChangeItem(SelectItem, SelectSlot);
			Inventory->MoveItem(SelectItem, SelectSlot);
			Cursor->SetCursorSize(SelectItem->GetItemSize());
		}
	}
	else if (SelectItem && FSMComp->GetCurrentFSMKey() != TO_KEY(EInventoryState::Select))
	{
		FVector Location = Inventory->GetItemWorldLocation(SelectItem);
		Location += FVector(SelectItem->GetItemSize().X * 5.0f + -12.5f, -10.0f, 0) ;
		FVector2D Pos;
		UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(this, 0), Location, Pos);
		BehaviorWidget->SetPositionInViewport(Pos);
		BehaviorWidget->SetItemData(SelectItem->GetData());
		FSMComp->ChangeState(TO_KEY(EInventoryState::Select));
	}
	}

void ABInventoryActor::Cancel()
{
	if (FSMComp->GetCurrentFSMKey() == TO_KEY(EInventoryState::Select))
	{
		FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
		BehaviorWidget->SetHide();
		return;
	}
	if (FSMComp->GetCurrentFSMKey() == TO_KEY(EInventoryState::Default))
	{
		UGameplayStatics::GetPlayerController(this, 0)->SetViewTarget(UGameplayStatics::GetPlayerPawn(this, 0));
		Subsystem->RemoveMappingContext(DefaultMappingContext);
		// 인벤토리 종료
		return;
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
	if (FSMComp->GetCurrentFSMKey() == TO_KEY(EInventoryState::Drag) && !bIsDragMove)
	{
		bIsDragMove = 1;
		Inventory->RaiseItem(SelectItem);
		Inventory->MoveItem(SelectItem, SelectSlot);
		Cursor->SetCursorRaise(true);
		Cursor->SetCursorPosition(SelectSlot);
		OpenSub();
	}
}

void ABInventoryActor::DragCancel()
{
	if (FSMComp->GetCurrentFSMKey() == TO_KEY(EInventoryState::Drag) && !bIsDragMove)
	{
		FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
	}
}

void ABInventoryActor::Turn()
{
	if (!bIsDragMove)
	{
		return;
	}

	SelectItem->Turn();
	Cursor->SetCursorSize(SelectItem->GetItemSize());
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
			Cursor->SetCursorPosition(SelectSlot);

			if (UBInventoryItem* Item = Slot->GetItem())
			{
				SelectItem = Item;
				Widget->SetItemData(Item->GetData());
				Cursor->SetCursorSize(Item->GetItemSize());
			}
			else
			{
				SelectItem = nullptr;
				Widget->ClearItemData();
				Cursor->SetCursorSize({1, 1});
			}

		}
	}
	else if (SelectSlot)
	{
		SelectSlot = nullptr;
		SelectItem = nullptr;
		Widget->ClearItemData();
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
		return;
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
		if (Slot && SelectSlot != Slot)
		{
			SelectSlot = Slot;
			// 일단 이동시켜놓고 있음.
			// 다른 아이템을 덮어씌우는 문제 있음
			if (bIsDragMove == 1)
			{
				Inventory->MoveItem(SelectItem, Slot);
				Cursor->SetCursorPosition(Slot->GetPosition(), Slot->IsSubSlot());
			}
		}
	}
}

void ABInventoryActor::DragExit()
{
	if (bIsDragMove)
	{
		SelectSlot = nullptr;
		SelectItem = nullptr;
	}
	bIsDragMove = 0;
	Cursor->SetCursorRaise(false);

	if (false == Inventory->HasItemInSubSlot())
	{
		CloseSub();
	}
}
