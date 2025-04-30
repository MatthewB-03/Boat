// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineMeshRenderer.h"

// Sets default values
ASplineMeshRenderer::ASplineMeshRenderer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateAbstractDefaultSubobject<USplineComponent>("Spline", false);
}

// Called when the game starts or when spawned
void ASplineMeshRenderer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASplineMeshRenderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move wave by speed
	WaveOffsetValue += WaveSpeed * DeltaTime;
	if (WaveOffsetValue > 360) 
	{
		WaveOffsetValue -= 360;
	}

	// Draw spline mesh
	DrawSplineMesh();
}

// Draws the spline mesh
void ASplineMeshRenderer::DrawSplineMesh()
{
	// Create spline
	Spline->ClearSplinePoints();
	FVector Point;
	for (int i = 0; i < NumberOfPoints; i++)
	{
		// Create spline point
		Point = (GetActorLocation() + GetActorForwardVector() * (float)PointSpacing * i);

		// Offset for wave effect
		Point += GetActorRightVector() * FMath::Sin((float)WaveLengthMultiplier * (float)PointSpacing * i + (float)WaveOffsetValue) * (float)WaveAmplitude;

		// Add spline point
		Spline->AddSplinePoint(Point, ESplineCoordinateSpace::Type::World, true);
	}

	// Clear previous spline meshes
	for (USplineMeshComponent* SplineMesh : SplineMeshes)
	{
		SplineMesh->DestroyComponent();
	}
	SplineMeshes.Empty();

	// Create new spline meshes
	for (int i = 0; i < Spline->GetNumberOfSplinePoints() - 1; i++)
	{
		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
		SplineMesh->SetStaticMesh(SplineBaseMesh);
		SplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SplineMesh->SetCollisionProfileName("NoCollision");
		SplineMesh->SetMaterial(0, SplineMeshMaterial);
		SplineMesh->Mobility = EComponentMobility::Movable;
		SplineMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMesh->SetForwardAxis(ESplineMeshAxis::Z);
		SplineMesh->SetStartPosition(Spline->GetTransformAtSplinePoint(i, ESplineCoordinateSpace::Type::World).GetLocation(), true);
		SplineMesh->SetEndPosition(Spline->GetTransformAtSplinePoint(i + 1, ESplineCoordinateSpace::Type::World).GetLocation(), true);
		SplineMesh->SetStartTangent(Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Type::World), true);
		SplineMesh->SetEndTangent(Spline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Type::World), true);
		SplineMesh->RegisterComponent();
		SplineMeshes.Add(SplineMesh);
	}
}

