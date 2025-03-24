// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRod.h"

// Sets default values
APlayerRod::APlayerRod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Mesh = CreateAbstractDefaultSubobject<USkeletalMeshComponent>("Mesh", false);

	// Set root transform
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void APlayerRod::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerRod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

