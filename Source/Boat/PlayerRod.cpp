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

	// Disable Collision & Queries on mesh
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionProfileName("NoCollision");

	// Add HUD to viewport
	PlayerHud = CreateWidget<UPlayerHudWidget>(GetWorld(), PlayerHudType);
	PlayerHud->AddToViewport();
	
}

// Called every frame
void APlayerRod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Set random seed
	std::chrono::system_clock::time_point Now = std::chrono::system_clock::now();
	FMath::SRandInit(Now.time_since_epoch().count());
	FMath::RandInit(Now.time_since_epoch().count());

	// Update current state
	StateTime += DeltaTime;
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

	// Draw spline mesh
	DrawSplineMesh();

	// Update UI
	PlayerHud->UpdateUI(DeltaTime);

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

	if (MouseIsDown && StateTime >= 0.1f) 
	{
		// Start drawing back rod
		CurrentState = RodState::Drawing;
		StateTime = 0;
		AnimInstance->BlendAlpha = 0;
	}
	LineEndActor->State = HookState::Idle;
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
		LineEndActor->State = HookState::Idle;
		LineEndActor->SetActorLocation(Mesh->GetSocketLocation("RodEndSocket"));
	}
	else 
	{
		// Cast rod
		CurrentState = RodState::Casting;
		StateTime = 0;
		FishWaitTime = FMath::RandRange(10.0f, 30.0f);
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
		StateTime = 0;
	}
	else if (LineEndActor->State == HookState::InWater) 
	{
		if (StateTime >= FishWaitTime) 
		{
			// Start reeling
			CurrentState = RodState::Reeling;
			StateTime = 0;
			FishWaitTime = FMath::RandRange(5.0f, 10.0f);
			FishType = GetRandomFish();
		}
	}

	// Create curved rope
	CreateSplineCurved();
}

// Called every frame in the reeling state
void APlayerRod::ReelingTick(float DeltaTime)
{
	if (MouseIsDown)
	{
		AnimInstance->BlendAlpha += DeltaTime * 2;
	}
	else
	{
		AnimInstance->BlendAlpha -= DeltaTime * 2;
	}

	if (StateTime >= FishWaitTime && CurrentState == RodState::Reeling)
	{
		CurrentState = RodState::Caught;
		StateTime = 0;
		PlayerHud->ShowCaughtText(FText::FromString(TEXT("Caught " + FishType.Name)));
		LineEndActor->SetModel(FishType.Mesh, FishType.Material);
		LineEndActor->State = HookState::Idle;
	}

	// Create straight rope
	CreateSplineStraight();
}

// Called every frame in the caught state
void APlayerRod::CaughtTick(float DeltaTime)
{
	// Create straight rope
	CreateSplineStraight();

	LineEndActor->SetActorLocation(Mesh->GetSocketLocation("RodEndSocket") + FVector(0, 0, CaughtFishZOffset));

	if (MouseIsDown && StateTime >= 1)
	{
		// Reset hook model
		LineEndActor->ResetModel();

		// Return to idle
		CurrentState = RodState::Idle;
		StateTime = 0;
	}
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

// Gets a random fish type from the array
FStrucFish APlayerRod::GetRandomFish() 
{
	float TotalWeight = 0.0f;
	for (int i = 0; i < FishTypes.Num(); i++) 
	{
		TotalWeight += 1/FishTypes[i].Rarity;
	}

	float RandomValue = FMath::RandRange(0.0f, TotalWeight);

	float CurrentWeight = 0.0f;
	for (int i = 0; i < FishTypes.Num(); i++)
	{
		CurrentWeight += 1 / FishTypes[i].Rarity;
		if (RandomValue <= CurrentWeight)
		{
			return FishTypes[i];
		}
	}

	return FishTypes[0];
}

// Defines and draws a straight spline mesh
void APlayerRod::CreateSplineStraight()
{
	// Clear spline
	Spline->ClearSplinePoints();

	// Redefine spline points
	Spline->ClearSplinePoints();
	Spline->AddSplinePoint(Mesh->GetSocketLocation("RodEndSocket"), ESplineCoordinateSpace::Type::World, true);
	Spline->AddSplinePoint(LineEndActor->GetActorLocation(), ESplineCoordinateSpace::Type::World, true);
}

// Defines and draws a curved spline mesh
void APlayerRod::CreateSplineCurved()
{
	// Clear spline
	Spline->ClearSplinePoints();

	// Redefine spline points
	Spline->ClearSplinePoints();
	Spline->AddSplinePoint(Mesh->GetSocketLocation("RodEndSocket"), ESplineCoordinateSpace::Type::World, true);
	Spline->AddSplinePoint(FMath::Lerp(LineEndActor->GetActorLocation(), Mesh->GetSocketLocation("RodEndSocket"), 0.5f) + FVector(0, 0, LineRopeDip), ESplineCoordinateSpace::Type::World, true);
	Spline->AddSplinePoint(LineEndActor->GetActorLocation(), ESplineCoordinateSpace::Type::World, true);
}

// Draws a spline mesh around the current spline
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
		SplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SplineMesh->SetCollisionProfileName("NoCollision");
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

