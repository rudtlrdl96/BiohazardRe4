// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"
#include "Generic/BFsm.h"
#include "Components/TextRenderComponent.h"

ABInventoryItem::ABInventoryItem()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(RootComponent);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRender->SetupAttachment(RootComponent);
	TextRender->SetText(FText::FromString(FString::FromInt(0)));
	TextRender->SetWorldSize(2.8);
	TextRender->SetRelativeRotation({ 90, 90, 0 });
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Game/Assets/Font/M_OutlineTextMaterialOpaque.M_OutlineTextMaterialOpaque'"));
	TextRender->SetTextMaterial(Material.Object);
	TextRender->SetHorizontalAlignment(EHorizTextAligment::EHTA_Right);
	TextRender->SetRenderCustomDepth(true);
	TextRender->SetCustomDepthStencilValue(1);

	// FSM
	FSMComp = CreateDefaultSubobject<UBFsm>(TEXT("FSM"));
	
	UBFsm::FStateCallback WaitState;
	UBFsm::FStateCallback MoveState;
	UBFsm::FStateCallback PutState;
	MoveState.EnterDel.BindUObject(this, &ABInventoryItem::MoveEnter);
	MoveState.UpdateDel.BindUObject(this, &ABInventoryItem::MoveUpdate);
	PutState.EnterDel.BindUObject(this, &ABInventoryItem::PutEnter);
	PutState.UpdateDel.BindUObject(this, &ABInventoryItem::PutUpdate);

	FSMComp->CreateState(TO_KEY(ItemState::Wait), WaitState);
	FSMComp->CreateState(TO_KEY(ItemState::Move), MoveState);
	FSMComp->CreateState(TO_KEY(ItemState::Put), PutState);

	FSMComp->ChangeState(TO_KEY(ItemState::Wait));
}

void ABInventoryItem::SetItemData(const FBItemData& _Data)
{
	Data = _Data;
	Mesh->RegisterComponent();
	Mesh->SetStaticMesh(_Data.Mesh);
	MeshLocation = _Data.Location;
	Mesh->SetRelativeLocation(_Data.Location);
	Mesh->SetRelativeRotation(_Data.Rotation);
	Mesh->SetRelativeScale3D(_Data.Scale);
	TextRender->SetRelativeLocation({ -3.0 + 5.0 * Data.ItemSize.X, -2.0 + 5.0 * Data.ItemSize.Y, 3.0 });
}

void ABInventoryItem::SetRaise()
{
	StartLocation = RootComponent->GetRelativeLocation();
	TargetLocation = StartLocation;
	FSMComp->ChangeState(TO_KEY(ItemState::Move));
}

void ABInventoryItem::SetMove(const FVector& _Location)
{
	StartLocation = RootComponent->GetRelativeLocation();
	TargetLocation = _Location;
	MoveAlpha = 0;
}

void ABInventoryItem::SetPut(const FVector& _Location)
{
	StartLocation = RootComponent->GetRelativeLocation();
	TargetLocation = _Location;
	FSMComp->ChangeState(TO_KEY(ItemState::Put));
}

void ABInventoryItem::Turn()
{
	bIsTurn = ~bIsTurn;
	bIsCurrentTurn = 1;
	TurnAlpha = 0;
	if (bIsTurn)
	{
		MeshStartLocation = Data.Location;
		MeshStartRotate = Data.Rotation;
		MeshTargetLocation = Data.TurnLocation;
		MeshTargetRotate = Data.TurnRotation;
		TextRender->SetRelativeLocation({ -3.0 + 5.0 * Data.ItemSize.Y, -2.0 + 5.0 * Data.ItemSize.X, 3.0 });
	}
	else
	{
		MeshStartLocation = Data.TurnLocation;
		MeshStartRotate = Data.TurnRotation;
		MeshTargetLocation = Data.Location;
		MeshTargetRotate = Data.Rotation;
		TextRender->SetRelativeLocation({ -3.0 + 5.0 * Data.ItemSize.X, -2.0 + 5.0 * Data.ItemSize.Y, 3.0 });
	}
}

void ABInventoryItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCurrentTurn)
	{
		TurnAlpha += DeltaTime * TurnSpeed;
		MeshLocation = FMath::Lerp(MeshStartLocation, MeshTargetLocation, TurnAlpha);
		Mesh->SetRelativeRotation(FMath::LerpRange(MeshStartRotate, MeshTargetRotate, TurnAlpha));
		if (1 < TurnAlpha)
		{
			Mesh->SetRelativeRotation(MeshTargetRotate);
			bIsCurrentTurn = 0;
		}
	}
}

void ABInventoryItem::MoveEnter()
{
	MoveAlpha = 0;
	RaiseAlpha = 0;
	StartLocation = RootComponent->GetRelativeLocation();
}

void ABInventoryItem::MoveUpdate(float DeltaTime)
{
	MoveAlpha = FMath::Min(1.0f, MoveAlpha + DeltaTime * MoveSpeed);
	RaiseAlpha = FMath::Min(1.0f, RaiseAlpha + DeltaTime * RaiseSpeed);
	
	RootComponent->SetRelativeLocation(FMath::Lerp<FVector>(StartLocation, TargetLocation, MoveAlpha));
	Mesh->SetRelativeLocation(FMath::Lerp<FVector>(MeshLocation, MeshLocation + FVector(0, 0, 5), RaiseAlpha));
}

void ABInventoryItem::PutEnter()
{
	MoveAlpha = 0;
	RaiseAlpha = 0;
	StartLocation = RootComponent->GetRelativeLocation();
}

void ABInventoryItem::PutUpdate(float DeltaTime)
{
	MoveAlpha = FMath::Min(1.0f, MoveAlpha + DeltaTime * MoveSpeed);
	RaiseAlpha = FMath::Min(1.0f, RaiseAlpha + DeltaTime * RaiseSpeed);

	RootComponent->SetRelativeLocation(FMath::Lerp<FVector>(StartLocation, TargetLocation, MoveAlpha));
	Mesh->SetRelativeLocation(FMath::Lerp<FVector>(MeshLocation + FVector(0, 0, 5), MeshLocation, RaiseAlpha));

	if (.99f <= RaiseAlpha)
	{
		RootComponent->SetRelativeLocation(TargetLocation);
		Mesh->SetRelativeLocation(MeshLocation);
		FSMComp->ChangeState(TO_KEY(ItemState::Wait));
	}
}
