// Fill out your copyright notice in the Description page of Project Settings.


#include "LineEnd.h"

// Sets default values
ALineEnd::ALineEnd()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Mesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>("Mesh", false);
	
	// Set root transform
	RootComponent = Mesh;

	// Define collision channel
	CollisionParams = FCollisionQueryParams();
	CollisionParams.bTraceComplex = true;
}

// Called when the game starts or when spawned
void ALineEnd::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetStaticMesh(HookModel);

	// Disable Collision & Queries on mesh
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionProfileName("NoCollision");
}

// Called every frame
void ALineEnd::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If falling
	if (State == HookState::Falling)
	{
		FHitResult Hit;

		// Check for horizontal collision
		if (GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), GetActorLocation() + Velocity.GetSafeNormal() * CollisionRadius, ECC_WorldStatic, CollisionParams))
		{
			// Bounce off collision
			Velocity.Z -= (abs(Velocity.Y) + abs(Velocity.X)) / 2;
			Velocity.X *= -0.5f;
			Velocity.Y *= -0.5f;
			SetActorLocation(Hit.ImpactPoint - Velocity.GetSafeNormal() * CollisionRadius);
		}

		// Check for vertical collision
		if (GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), GetActorLocation() + FVector(0, 0, -1) * CollisionRadius, ECC_WorldStatic, CollisionParams))
		{
			// Come to a rest
			Velocity = FVector(0, 0, 0);
			State = HookState::Idle;
			SetActorLocation(Hit.ImpactPoint - FVector(0, 0, -1) * CollisionRadius);
			UE_LOG(LogClass, Log, TEXT("Hook Hit: %s"), *Hit.GetActor()->GetName());
		}

		// If still falling
		if (State == HookState::Falling)
		{
			// Move by velocity
			Velocity += FVector(0, 0, -FallAcceleration * DeltaTime);
			SetActorLocation(GetActorLocation() + Velocity * DeltaTime);

			// Check if landed in water
			if (GetActorLocation().Z <= SeaHeight)
			{
				Velocity = FVector(0, 0, 0);
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, SeaHeight));
				State = HookState::InWater;
			}
		}
	}
}

// Casts the line in the given direction
void ALineEnd::Cast(FVector Direction)
{
	Velocity = Direction * CastSpeed;
	State = HookState::Falling;
}



