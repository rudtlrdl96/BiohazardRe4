// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/BDrawGrenadeAim.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BiohazardRe4.h"

const int32 ABDrawGrenadeAim::MaxMeshCount = 50;

// Sets default values
ABDrawGrenadeAim::ABDrawGrenadeAim()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	RootComponent = SplineComp;
		
	SplineMeshs.SetNum(MaxMeshCount);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshRef(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> AimMatRef(TEXT("/Script/Engine.Material'/Game/Blueprints/Actor/Player/Weapon/Grenade/M_GrenadeAim.M_GrenadeAim'"));

	for (int32 i = 0; i < MaxMeshCount; ++i)
	{
		FString NameString = FString(TEXT("Spline Mesh"));
		NameString.AppendChar('-');
		NameString.AppendInt(i);

		FName ToName = FName(*NameString);

		SplineMeshs[i] = CreateDefaultSubobject<USplineMeshComponent>(ToName);
		SplineMeshs[i]->SetStaticMesh(CubeMeshRef.Object);
		SplineMeshs[i]->SetMaterial(0, AimMatRef.Object);
		SplineMeshs[i]->Mobility = EComponentMobility::Type::Movable;
		SplineMeshs[i]->ForwardAxis = ESplineMeshAxis::Type::X;

		SplineMeshs[i]->SetStartScale(FVector2D(0.07, 0.07));
		SplineMeshs[i]->SetEndScale(FVector2D(0.07, 0.07));

		if (nullptr == SplineMeshs[i])
		{
			LOG_FATAL(TEXT("Fail Create SplineMesh"));
		}

		SplineMeshs[i]->SetVisibility(false);
	}

	DecalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Decal Mesh"));
	DecalMesh->SetStaticMesh(CubeMeshRef.Object);
	DecalMesh->SetMaterial(0, AimMatRef.Object);
	DecalMesh->SetCollisionProfileName("NoCollision");
	DecalMesh->Mobility = EComponentMobility::Type::Movable;
	DecalMesh->SetVisibility(false);
}

// Called when the game starts or when spawned
void ABDrawGrenadeAim::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABDrawGrenadeAim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABDrawGrenadeAim::Draw(const FPredictProjectilePathResult& _Path, bool _bIsSplineDraw)
{
	DisableDraw();

	int32 EndIndex = FMath::Min(MaxMeshCount, _Path.PathData.Num());

	SplineComp->ClearSplinePoints();

	for (int i = 0; i < EndIndex; ++i)
	{		
		SplineComp->AddSplinePoint(_Path.PathData[i].Location, ESplineCoordinateSpace::Type::World);
	}

	if (true == _bIsSplineDraw)
	{
		for (int i = 0; i < EndIndex - 1; ++i)
		{
			FVector StartLocation = SplineComp->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Type::World);
			FVector StartTangent = SplineComp->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Type::World);

			FVector EndLocation = SplineComp->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Type::World);
			FVector EndTangent = SplineComp->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Type::World);

			SplineMeshs[i]->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent);
			SplineMeshs[i]->SetVisibility(true);
		}
	}

	if (false == _Path.HitResult.bBlockingHit)
	{
		return;
	}

	DecalMesh->SetWorldLocation(_Path.HitResult.Location);
	DecalMesh->SetWorldRotation(_Path.HitResult.ImpactNormal.Rotation() + FRotator(-90, 0, 0));
	DecalMesh->SetVisibility(true);
}

void ABDrawGrenadeAim::DisableDraw()
{
	for (int i = 0; i < SplineMeshs.Num(); ++i)
	{
		SplineMeshs[i]->SetVisibility(false);
	}

	DecalMesh->SetVisibility(false);
}