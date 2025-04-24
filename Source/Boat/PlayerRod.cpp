// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRod.h"

// Sets default values
APlayerRod::APlayerRod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Mesh = CreateAbstractDefaultSubobject<USkeletalMeshComponent>("Mesh", false);
	Spline = CreateAbstractDefaultSubobject<USplineComponent>("Spline", false);
	LineEndChildActor = CreateAbstractDefaultSubobject<UChildActorComponent>("LineEndChildActor", false);

	// Set root transform
	RootComponent = Mesh;

	// Attach Line End
	LineEndChildActor->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void APlayerRod::BeginPlay()
{
	Super::BeginPlay();

	// Get animation instance
	AnimInstance = Cast<URodAnimInstance>(Mesh->GetAnimInstance());

	// Get line end actor
	LineEndActor = Cast<ALineEnd>(LineEndChildActor->GetChildActor());
	
}

// Called every frame
void APlayerRod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Redefine spline points
	Spline->ClearSplinePoints();
	Spline->AddSplinePoint(Mesh->GetSocketLocation("RodEndSocket"), ESplineCoordinateSpace::Type::World, true);
	Spline->AddSplinePoint(FMath::Lerp(LineEndActor->GetActorLocation(), Mesh->GetSocketLocation("RodEndSocket"), 0.5f)+FVector(0, 0, -20), ESplineCoordinateSpace::Type::World, true);
	Spline->AddSplinePoint(LineEndActor->GetActorLocation(), ESplineCoordinateSpace::Type::World, true);


	// Draw spline mesh
	DrawSplineMesh();

	// Update current state
	if (CurrentState == RodState::Idle)
	{
		IdleTick(DeltaTime);
	}
	else if (CurrentState == RodState::Drawing)
	{
		DrawingTick(DeltaTime);
	}
	else if (CurrentState == RodState::Casting)
	{
		CastingTick(DeltaTime);
	}
	else if (CurrentState == RodState::Reeling)
	{
		ReelingTick(DeltaTime);
	}
	else if (CurrentState == RodState::Caught)
	{
		CaughtTick(DeltaTime);
	}

	// Update animation state
	AnimInstance->CurrentState = CurrentState;

	// Clamp blend alpha between 0 and 1
	if (AnimInstance->BlendAlpha < 0)
	{
		AnimInstance->BlendAlpha = 0;
	}
	else if (AnimInstance->BlendAlpha > 1)
	{
		AnimInstance->BlendAlpha = 1;
	}
}

// Called every frame in the idle state
void APlayerRod::IdleTick(float DeltaTime)
{
	// Clear spline
	Spline->ClearSplinePoints();

	if (MouseIsDown) 
	{
		// Start drawing back rod
		CurrentState = RodState::Drawing;
		AnimInstance->BlendAlpha = 0;
	}
	LineEndActor->Falling = false;
	LineEndActor->SetActorLocation(Mesh->GetSocketLocation("RodEndSocket"));
}

// Called every frame in the drawing state
void APlayerRod::DrawingTick(float DeltaTime)
{
	// Clear spline
	Spline->ClearSplinePoints();

	if (MouseIsDown)
	{
		// Increase draw strength over time
		AnimInstance->BlendAlpha += DeltaTime * DrawSpeed;
		LineEndActor->Falling = false;
		LineEndActor->SetActorLocation(Mesh->GetSocketLocation("RodEndSocket"));
	}
	else 
	{
		// Cast rod
		CurrentState = RodState::Casting;
		LineEndActor->Cast(GetActorForwardVector()*(0.5f + AnimInstance->BlendAlpha*2));
	}
}

// Called every frame in the casting state
void APlayerRod::CastingTick(float DeltaTime)
{
	if (MouseIsDown)
	{
		// Cancel cast wait
		CurrentState = RodState::Idle;
	}
	// Redefine spline points
	Spline->ClearSplinePoints();
	Spline->AddSplinePoint(Mesh->GetSocketLocation("RodEndSocket"), ESplineCoordinateSpace::Type::World, true);
	Spline->AddSplinePoint(FMath::Lerp(LineEndActor->GetActorLocation(), Mesh->GetSocketLocation("RodEndSocket"), 0.5f) + FVector(0, 0, -20), ESplineCoordinateSpace::Type::World, true);
	Spline->AddSplinePoint(LineEndActor->GetActorLocation(), ESplineCoordinateSpace::Type::World, true);


	// Draw spline mesh
	DrawSplineMesh();
}

// Called every frame in the reeling state
void APlayerRod::ReelingTick(float DeltaTime)
{
	// Redefine spline points
	Spline->ClearSplinePoints();
	Spline->AddSplinePoint(Mesh->GetSocketLocation("RodEndSocket"), ESplineCoordinateSpace::Type::World, true);
	Spline->AddSplinePoint(FMath::Lerp(LineEndActor->GetActorLocation(), Mesh->GetSocketLocation("RodEndSocket"), 0.5f) + FVector(0, 0, -20), ESplineCoordinateSpace::Type::World, true);
	Spline->AddSplinePoint(LineEndActor->GetActorLocation(), ESplineCoordinateSpace::Type::World, true);


	// Draw spline mesh
	DrawSplineMesh();
}

// Called every frame in the caught state
void APlayerRod::CaughtTick(float DeltaTime)
{
	// Redefine spline points
	Spline->ClearSplinePoints();
	Spline->AddSplinePoint(Mesh->GetSocketLocation("RodEndSocket"), ESplineCoordinateSpace::Type::World, true);
	Spline->AddSplinePoint(FMath::Lerp(LineEndActor->GetActorLocation(), Mesh->GetSocketLocation("RodEndSocket"), 0.5f) + FVector(0, 0, -20), ESplineCoordinateSpace::Type::World, true);
	Spline->AddSplinePoint(LineEndActor->GetActorLocation(), ESplineCoordinateSpace::Type::World, true);


	// Draw spline mesh
	DrawSplineMesh();
}

// Called when the mouse is clicked down
void APlayerRod::MouseDown()
{
	MouseIsDown = true;
	UE_LOG(LogTemp, Warning, TEXT("Clicked"));
}

// Called when the mouse click is released
void APlayerRod::MouseUp()
{
	MouseIsDown = false;
	UE_LOG(LogTemp, Warning, TEXT("Released"));
}

// Draws the spline mesh
void APlayerRod::DrawSplineMesh() 
{
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
		SplineMesh->SetMaterial(0, SplineMeshMaterial);
		SplineMesh->Mobility = EComponentMobility::Movable;
		SplineMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMesh->SetForwardAxis(ESplineMeshAxis::Z);
		SplineMesh->SetStartPosition(Spline->GetTransformAtSplinePoint(i, ESplineCoordinateSpace::Type::World).GetLocation(), true);
		SplineMesh->SetEndPosition(Spline->GetTransformAtSplinePoint(i+1, ESplineCoordinateSpace::Type::World).GetLocation(), true);
		SplineMesh->SetStartTangent(Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Type::World), true);
		SplineMesh->SetEndTangent(Spline->GetTangentAtSplinePoint(i+1, ESplineCoordinateSpace::Type::World), true);
		SplineMesh->RegisterComponent();
		SplineMeshes.Add(SplineMesh);
	}
}

