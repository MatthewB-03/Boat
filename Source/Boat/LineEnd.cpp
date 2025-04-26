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
}

// Called when the game starts or when spawned
void ALineEnd::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetStaticMesh(HookModel);
}

// Called every frame
void ALineEnd::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Falling)
	{
		Velocity += FVector(0, 0, -FallAcceleration * DeltaTime);
		SetActorLocation(GetActorLocation() + Velocity * DeltaTime);
		if (GetActorLocation().Z < SeaHeight)
		{
			Falling = false;
		}
	}
}

// Casts the line in the given direction
void ALineEnd::Cast(FVector Direction)
{
	Velocity = Direction * CastSpeed;
	Falling = true;
}



