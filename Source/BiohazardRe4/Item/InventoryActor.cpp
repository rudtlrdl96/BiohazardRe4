// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryActor.h"
#include "InventoryManager.h"
#include "InventorySlot.h"
#include "InventoryWeapon.h"
#include "InventoryWidgetMain.h"
#include "InventoryWidgetBehavior.h"
#include "InventoryWidgetCraft.h"
#include "InventoryWidgetQuickSlot.h"
#include "InventoryCursor.h"
#include "Actor/Player/HUD/HUDBase.h"
#include "BiohazardRe4.h"
#include "Generic/BFsm.h"
#include "Actor/Player/Body/BLeon.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ABInventoryActor* ABInventoryActor::Instance = nullptr;

// Sets default values
ABInventoryActor::ABInventoryActor()
{
	Instance = this;
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

	InvestigatePivot = CreateDefaultSubobject<USceneComponent>(TEXT("InvestigatePivot"));
	InvestigatePivot->SetupAttachment(RootComponent);

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

	// 퀵슬롯
	QuickSlot.SetNum(8);

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

	// FSM설정
	UBFsm::FStateCallback DefaultState;
	DefaultState.EnterDel.BindUObject(this,	&ABInventoryActor::DefaultEnter);
	DefaultState.UpdateDel.BindUObject(this, &ABInventoryActor::DefaultUpdate);
	DefaultState.ExitDel.BindUObject(this, &ABInventoryActor::DefaultExit);
	UBFsm::FStateCallback DragState;
	DragState.EnterDel.BindUObject(this, &ABInventoryActor::DragEnter);
	DragState.UpdateDel.BindUObject(this, &ABInventoryActor::DragUpdate);
	DragState.ExitDel.BindUObject(this, &ABInventoryActor::DragExit);
	UBFsm::FStateCallback SelectState;
	SelectState.EnterDel.BindUObject(this, &ABInventoryActor::SelectEnter);
	SelectState.ExitDel.BindUObject(this, &ABInventoryActor::SelectExit);
	UBFsm::FStateCallback InvestigateState;
	InvestigateState.EnterDel.BindUObject(this, &ABInventoryActor::InvestigateEnter);
	InvestigateState.UpdateDel.BindUObject(this, &ABInventoryActor::InvestigateUpdate);
	InvestigateState.ExitDel.BindUObject(this, &ABInventoryActor::InvestigateExit);
	UBFsm::FStateCallback CraftState;
	CraftState.EnterDel.BindUObject(this, &ABInventoryActor::CraftEnter);
	CraftState.ExitDel.BindUObject(this, &ABInventoryActor::CraftExit);
	UBFsm::FStateCallback QuickSlotCallBack;
	QuickSlotCallBack.EnterDel.BindUObject(this, &ABInventoryActor::QuickSlotEnter);
	QuickSlotCallBack.ExitDel.BindUObject(this, &ABInventoryActor::QuickSlotExit);
	UBFsm::FStateCallback DropCallBack;
	DropCallBack.EnterDel.BindUObject(this, &ABInventoryActor::DropEnter);
	DropCallBack.ExitDel.BindUObject(this, &ABInventoryActor::DropExit);
	UBFsm::FStateCallback CloseCheckCallBack;
	CloseCheckCallBack.EnterDel.BindUObject(this, &ABInventoryActor::CloseCheckEnter);
	CloseCheckCallBack.ExitDel.BindUObject(this, &ABInventoryActor::CloseCheckExit);
	UBFsm::FStateCallback EmptyCallback;

	FSMComp->CreateState(TO_KEY(EInventoryState::Wait), EmptyCallback);
	FSMComp->CreateState(TO_KEY(EInventoryState::Default), DefaultState);
	FSMComp->CreateState(TO_KEY(EInventoryState::Drag), DragState);
	FSMComp->CreateState(TO_KEY(EInventoryState::Select), SelectState);
	FSMComp->CreateState(TO_KEY(EInventoryState::Craft), CraftState);
	FSMComp->CreateState(TO_KEY(EInventoryState::Drop), DropCallBack);
	FSMComp->CreateState(TO_KEY(EInventoryState::CloseCheck), CloseCheckCallBack);
	FSMComp->CreateState(TO_KEY(EInventoryState::Investigate), InvestigateState);
	FSMComp->CreateState(TO_KEY(EInventoryState::QuickSlot), QuickSlotCallBack);
	FSMComp->ChangeState(TO_KEY(EInventoryState::Wait));
}

// Called when the game starts or when spawned
void ABInventoryActor::BeginPlay()
{
	Super::BeginPlay();

	// 0번 PlayerController를 받아온다
	// UEnhancedInputLocalPlayerSubsystem을 받아온다
	// Subsystem에 MappingContext를 추가한다 (우선순위를 1번으로 두어서 0번으로 Mapping한 조작을 무시한다)
	Controller = UGameplayStatics::GetPlayerController(this, 0);
	HUD = Cast<ABHUDBase>(Controller->GetHUD());
	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Controller->GetLocalPlayer());
	Player = Cast<ABLeon>(UGameplayStatics::GetPlayerCharacter(this, 0));
	//Subsystem->AddMappingContext(DefaultMappingContext, 1);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(Controller->InputComponent);
	Input->BindAction(SelectAction, ETriggerEvent::Triggered, this, &ABInventoryActor::Click);
	Input->BindAction(DragAction, ETriggerEvent::Started, this, &ABInventoryActor::DragStart);
	Input->BindAction(DragAction, ETriggerEvent::Triggered, this, &ABInventoryActor::DragTrigger);
	Input->BindAction(DragAction, ETriggerEvent::Canceled, this, &ABInventoryActor::DragCancel);
	Input->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ABInventoryActor::Turn);
	Input->BindAction(CancelAction, ETriggerEvent::Triggered, this, &ABInventoryActor::Cancel);
	Input->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ABInventoryActor::InvestigateRotate);

	// 위젯 생성
	Widget = CreateWidget<UBInventoryWidgetMain>(GetWorld(), InventoryWidgetClass);
	Widget->AddToViewport();
	Widget->WidgetOff();

	BehaviorWidget = CreateWidget<UBInventoryWidgetBehavior>(GetWorld(), BehaviorWidgetClass);
	BehaviorWidget->AddToViewport();
	BehaviorWidget->InventoryActor = this;

	CraftWidget = CreateWidget<UBInventoryWidgetCraft>(GetWorld(), CraftWidgetClass);
	CraftWidget->AddToViewport();
	
	QuickSlotWidget = CreateWidget<UBInventoryWidgetQuickSlot>(GetWorld(), QuickSlotWidgetClass);
	QuickSlotWidget->AddToViewport();

	// 타임라인 설정 (SubCase 애니메이션)
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

void ABInventoryActor::AddItem(EItemCode ItemCode, int Num)
{
	if (ItemCode == EItemCode::Money)
	{
		AddMoney(Num);
		return;
	}
	Inventory->AddItem(ItemCode, Num);
	Widget->AddItem(ItemCode, Num);
	HUD->UpdateStoredAmmo();
}

void ABInventoryActor::AddItemRowName(FName ItemRowName, int Num)
{
	Inventory->AddItem(ItemRowName, Num);
	Widget->AddItemRowName(ItemRowName, Num);
}

void ABInventoryActor::AddMoney(int Num)
{
	Widget->SetMoney(Money);
	Money += Num;
	Widget->AddMoney(Num);
}

int ABInventoryActor::GetItemCount(EItemCode ItemCode) const
{
	return Inventory->GetItemCount(ItemCode);
}

void ABInventoryActor::RemoveItem(EItemCode ItemCode, int Num)
{
	Inventory->RemoveItem(ItemCode, Num);
}

ABInventoryWeapon* ABInventoryActor::GetQuickSlot(int SlotNum)
{
	return QuickSlot[SlotNum];
}

ABInventoryWeapon* ABInventoryActor::FindWeapon(EItemCode ItemCode)
{
	return Inventory->FindWeapon(ItemCode);
}

bool ABInventoryActor::IsEmptySlot(EItemCode ItemCode, int Count)
{
	return Inventory->IsEmptySlot(ItemCode, Count);
}

// 인벤토리UI 켜기
void ABInventoryActor::OpenInventory()
{
	Timeline.SetNewTime(0);		// SubCase의 위치를 조정
	CaseMesh->PlayAnimation(OpenAnim, false);	// 애니메이션 재생
	HUD->SetQuickSlotVisible(false);
	Widget->WidgetOn();
	Widget->SetMoney(Money);
	Subsystem->AddMappingContext(DefaultMappingContext, 1);	// 매핑컨텍스트 추가해서 조작 할 수 있게 만듬
	Controller->SetViewTarget(this);	// 뷰타겟을 이 엑터로 지정
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	Controller->SetInputMode(InputMode);
	Controller->SetShowMouseCursor(true);
	FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
	Inventory->Open();

	Player->DisableInput(Controller);

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

void ABInventoryActor::OpenCraft()
{
	// State 변경
	FSMComp->ChangeState(TO_KEY(EInventoryState::Craft));
}

void ABInventoryActor::CompleteCraft()
{
	// 제작 완료시 Default 상태로 돌아감
	SelectItem = nullptr;
	SelectSlot = nullptr;
	FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
}

void ABInventoryActor::ItemUse()
{
	// SelectItem을 사용
	// 아직 Item종류별 효과를 처리하지 않음
	// 아이템 사용후 Default 상태로 돌아감
	switch (SelectItem->GetItemCode())
	{
	case EItemCode::GreenHerb:
		Player->TakeHeal(400);
		break;
	case EItemCode::MixedHerb_GG:
		Player->TakeHeal(800);
		break;
	case EItemCode::MixedHerb_GR:
		Player->TakeHeal(1500);
		break;
	case EItemCode::FirstAidSpray:
		Player->TakeHeal(9999);
		break;
	}
	Inventory->RemoveItem(SelectItem);
	SelectItem = nullptr;
	SelectSlot = nullptr;
	FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
}

void ABInventoryActor::DropItem()
{
	// 아이템 버리기를 선택하여
	// 버리기를 확인하는 문구를 표시.
	//Widget->SetFocus();
	Widget->DropItem();
	// State를 변경
	FSMComp->ChangeState(TO_KEY(EInventoryState::Drop));
}

void ABInventoryActor::WeaponEquip()
{
	ABInventoryWeapon* Weapon = Cast<ABInventoryWeapon>(SelectItem);
	if (!Weapon)
	{
		LOG_ERROR(TEXT("Error, Select Item is Not Weapon class"))
		return;
	}
	Player->ChangeUseWeapon(Weapon->GetItemCode());
	HUD->SetWeapon(Weapon);
	FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
}

void ABInventoryActor::CompleteDrop()
{
	// 아이템 버리기 확인을 하여 SelectItem을 버림
	// Default 상태로 돌아감
	Inventory->RemoveItem(SelectItem);
	SelectItem = nullptr;
	SelectSlot = nullptr;
	FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
}

void ABInventoryActor::DropCancel()
{
	// 버리기를 취소하여 Select상태로 돌아감
	FSMComp->ChangeState(TO_KEY(EInventoryState::Select));
}

void ABInventoryActor::CloseCancel()
{
	// 아이템을 버리고 인벤토리를 닫는 선택을 취소함
	FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
}

void ABInventoryActor::CloseInventory()
{
	// 인벤토리를 닫는다
	// Subslot에 있는 아이템은 버림
	Inventory->RemoveAllItemInSubSlot();
	Widget->WidgetOff();
	Controller->SetViewTarget(UGameplayStatics::GetPlayerPawn(this, 0));	// ViewTarget 전환
	Controller->SetInputMode(FInputModeGameOnly());
	Controller->SetShowMouseCursor(false);
	Subsystem->RemoveMappingContext(DefaultMappingContext);		// MappingContext 제거하여 조작 끔
	HUD->QuickSlotUpdate(QuickSlot);
	FSMComp->ChangeState(TO_KEY(EInventoryState::Wait));
	Inventory->Close();

	Player->EnableInput(Controller);
}

void ABInventoryActor::StartInvestigate()
{
	FSMComp->ChangeState(TO_KEY(EInventoryState::Investigate));
}

void ABInventoryActor::EndInvestigate()
{
	FSMComp->ChangeState(TO_KEY(EInventoryState::Select));
}

void ABInventoryActor::OpenQuickSlot()
{
	FSMComp->ChangeState(TO_KEY(EInventoryState::QuickSlot));
}

void ABInventoryActor::CloseQuickSlot()
{
	FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
}

void ABInventoryActor::SetHealPreview()
{
	if (nullptr == SelectItem) { return; }
	float HP = Player->Stat.CurrentHp;
	switch (SelectItem->GetItemCode())
	{
	case EItemCode::GreenHerb:
		HP += 400;
		break;
	case EItemCode::MixedHerb_GG:
		HP += 800;
		break;
	case EItemCode::MixedHerb_GR:
		HP += 1500;
		break;
	case EItemCode::FirstAidSpray:
		HP += 9999;
		break;
	}
	HUD->SetHealPreview(FMath::Min(HP / Player->Stat.MaxHp, 1));
}

void ABInventoryActor::OffHealPreview()
{
	HUD->OffHealPreview();
}

void ABInventoryActor::Click()
{
	// 클릭 시 실행 (일정 시간 이내에 마우스를 누르고 땜)
	int32 Key = FSMComp->GetCurrentFSMKey();

	if (Key == TO_KEY(EInventoryState::Drag) && !bIsDragMove)
	{
		// Drag상태이지만 충분히 지속되지 않음 (아이템을 이동시키지 않음)
		Key = TO_KEY(EInventoryState::Default);
	}
	if (bIsDragMove)
	{
		// Drag상태에 충분히 지속됨	(아이템을 이동시킴)
		if (Inventory->IsEmptySlot(SelectSlot, SelectItem))
		{
			// 이동시킨 아이템이 내려놓을 수 있다면
			Inventory->MoveItemConfirm(SelectItem, SelectSlot);			// 아이템을 내려놓는다
			FSMComp->ChangeState(TO_KEY(EInventoryState::Default));		// Default 상태로
		}
		else if (Inventory->CheckChange(SelectItem, SelectSlot))
		{
			// 이동시킨 아이템을 내려놓고 다른 아이템을 받아올 수 있는 상태 (교환가능상태)
			SelectItem = Inventory->ChangeItem(SelectItem, SelectSlot);		// 아이템을 교환한다
			Inventory->MoveItem(SelectItem, SelectSlot);
			Cursor->SetCursorSize(SelectItem->GetItemSize());
		}
	}
	else if (SelectItem && Key == TO_KEY(EInventoryState::Default))
	{
		// 커서가 아이템을 가르키고 있는 경우
		// 해당 아이템을 선택하는 상태로 전환
		FVector Location = Inventory->GetItemWorldLocation(SelectItem);
		Location += FVector(SelectItem->GetItemSize().X * 5.0f + -12.5f, -10.0f, 0) ;
		FVector2D Pos;
		UGameplayStatics::ProjectWorldToScreen(Controller, Location, Pos);
		BehaviorWidget->SetPositionInViewport(Pos);
		BehaviorWidget->SetItemData(SelectItem->GetData());
		FSMComp->ChangeState(TO_KEY(EInventoryState::Select));
	}
}

void ABInventoryActor::Cancel()
{
	// 취소키를 누를떄 실행
	int32 Key = FSMComp->GetCurrentFSMKey();

	if (Key == TO_KEY(EInventoryState::Select))
	{
		// 선택하는 상태일때는 기본상태로 돌아온다
		FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
		return;
	}
	if (Key == TO_KEY(EInventoryState::Default))
	{
		if (Inventory->HasItemInSubSlot())
		{
			Widget->CloseCheck();
			FSMComp->ChangeState(TO_KEY(EInventoryState::CloseCheck));
			return;
		}
		// 인벤토리 종료
		CloseInventory();
		return;
	}
	if (Key == TO_KEY(EInventoryState::CloseCheck))
	{
		CloseCancel();
		return;
	}
	if (Key == TO_KEY(EInventoryState::Craft) || Key == TO_KEY(EInventoryState::Investigate) || Key == TO_KEY(EInventoryState::QuickSlot))
	{
		FSMComp->ChangeState(TO_KEY(EInventoryState::Select));
		return;
	}
	if (Key == TO_KEY(EInventoryState::Drop))
	{
		DropCancel();
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
	// 마우스를 일정시간 누르고 있으면 실행

	if (FSMComp->GetCurrentFSMKey() == TO_KEY(EInventoryState::Drag) && !bIsDragMove)
	{
		// 현재 드래그 상태라면 아이템을 이동시킬 수 있게 한다
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
	//Widget->SetFocus();
}

void ABInventoryActor::DefaultUpdate(float _DeltaTime)
{
	FHitResult Res;
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

			if (ABInventoryItem* Item = Slot->GetItem())
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

void ABInventoryActor::SelectEnter()
{
	//BehaviorWidget->SetFocus();
	BehaviorWidget->WidgetOn();
}

void ABInventoryActor::SelectExit()
{
	BehaviorWidget->WidgetOff();
	OffHealPreview();
}

static FVector StartLocation;
static FVector EndLocation;
static FRotator  StartRot;
static FRotator  EndRot;
static float Timer;
void ABInventoryActor::InvestigateEnter()
{
	//Widget->SetFocus();

	StartLocation = SelectItem->Mesh->GetComponentLocation();
	EndLocation = InvestigatePivot->GetComponentLocation();
	StartRot = SelectItem->Mesh->GetComponentRotation();
	EndRot = SelectItem->GetData().Rotation;
	Timer = 0;
	Inventory->Close();
}	


void ABInventoryActor::InvestigateUpdate(float DeltaTime)
{
	Timer = FMath::Min(1.0f, Timer + DeltaTime * 3.0f);
	SelectItem->Mesh->SetWorldLocation(FMath::Lerp(StartLocation, EndLocation, Timer));
	SelectItem->Mesh->SetWorldRotation(FQuat::Slerp(StartRot.Quaternion(), EndRot.Quaternion(), Timer));
}

void ABInventoryActor::InvestigateExit()
{
	// 버그생김 조합한 허브에서 오류생겼음
	SelectItem->Mesh->SetWorldLocation(StartLocation);
	SelectItem->Mesh->SetWorldRotation(StartRot);
	Inventory->Open();
}

void ABInventoryActor::InvestigateRotate(const FInputActionInstance& _MoveAction)
{
	if (FSMComp->GetCurrentFSMKey() == TO_KEY(EInventoryState::Investigate))
	{
		FVector2D Vector2D = _MoveAction.GetValue().Get<FVector2D>();
		FRotator Rot = FRotator(Vector2D.X, 0, Vector2D.Y) * 10;
		EndRot += Rot;
	}
}

void ABInventoryActor::CraftEnter()
{
// CraftWidget 위치 이동 및 표시
	FVector Location = Inventory->GetItemWorldLocation(SelectItem);
	Location += FVector(SelectItem->GetItemSize().X * 5.0f + -12.5f, -10.0f, 0);
	FVector2D Pos;
	UGameplayStatics::ProjectWorldToScreen(Controller, Location, Pos);
	CraftWidget->SetPositionInViewport(Pos);
	CraftWidget->SetItemData(SelectItem->GetData());
	//CraftWidget->SetFocus();
	CraftWidget->WidgetOn();
}

void ABInventoryActor::CraftExit()
{
	CraftWidget->WidgetOff();
	Widget->SetFocus();
}

void ABInventoryActor::QuickSlotEnter()
{
	ABInventoryWeapon* Weapon = Cast<ABInventoryWeapon>(SelectItem);

	if (!Weapon)
	{
		FSMComp->ChangeState(TO_KEY(EInventoryState::Default));
		return;
	}

	QuickSlotWidget->AddWeaponPtr = Weapon;
	//QuickSlotWidget->SetFocus();
	QuickSlotWidget->WidgetOn();
}

void ABInventoryActor::QuickSlotExit()
{
	QuickSlotWidget->WidgetOff();
}

void ABInventoryActor::DropEnter()
{
	Widget->DropItem();
	Widget->DropWidgetOn();
}

void ABInventoryActor::DropExit()
{
	Widget->DropWidgetOff();
}

void ABInventoryActor::CloseCheckEnter()
{
	Widget->CloseCheck();
	Widget->DropWidgetOn();
}

void ABInventoryActor::CloseCheckExit()
{
	Widget->DropWidgetOff();
}

void ABInventoryActor::RemoveQuickSlot(EItemCode Code)
{
	switch (Code)
	{
	case EItemCode::Handgun_SR09R:
	case EItemCode::Shotgun_W870:
	case EItemCode::Rifle_SRM1903:
	case EItemCode::Grenade:
	case EItemCode::Flashbang:
		break;
	default:
		return;
	}

	for (int i = 0; i < 8; i++)
	{
		if (QuickSlot[i]->GetItemCode() == Code)
		{
			QuickSlot[i] = nullptr;
			return;
		}
	}
}

void ABInventoryActor::RemoveQuickSlot(ABInventoryItem* Item)
{
	if (ABInventoryWeapon* Weapon = Cast<ABInventoryWeapon>(Item))
	{
		for (int i = 0; i < 8; i++)
		{
			if (QuickSlot[i] == Weapon)
			{
				QuickSlot[i] = nullptr;
				return;
			}
		}
	}

}
