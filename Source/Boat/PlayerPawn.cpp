// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Create components
	Transform = CreateAbstractDefaultSubobject<USceneComponent>("Scene", false);
	Camera = CreateAbstractDefaultSubobject<UCameraComponent>("Camera", false);

	// Set root transform
	RootComponent = Transform;

	// Auto possess player 0
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// Enable player input
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// Bind axes
	InputComponent->BindAxis("MoveForward");
	InputComponent->BindAxis("MoveRight");
	InputComponent->BindAxis("LookUp");
	InputComponent->BindAxis("LookRight");
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get input values
	MoveForward = GetInputAxisValue("MoveForward");
	MoveRight = GetInputAxisValue("MoveRight");
	LookUp = GetInputAxisValue("LookUp");
	LookRight = GetInputAxisValue("LookRight");

	// Move player
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * MoveForward * MoveSpeed * DeltaTime * FVector(1.0f, 1.0f, 0.0f));
	SetActorLocation(GetActorLocation() + GetActorRightVector() * MoveRight * MoveSpeed * DeltaTime * FVector(1.0f, 1.0f, 0.0f));

	// Rotate player
	SetActorRotation(GetActorRotation() + LookUp * TurnSpeed * DeltaTime * FRotator(1.0f, 0.0f, 0.0f));
	SetActorRotation(GetActorRotation() + LookRight * TurnSpeed * DeltaTime * FRotator(0.0f, 1.0f, 0.0f));

	// Max/Min looking pitch
	if (GetActorRotation().Pitch > 45)
		SetActorRotation(FRotator(45, GetActorRotation().Yaw, GetActorRotation().Roll));
	else if (GetActorRotation().Pitch < -45)
		SetActorRotation(FRotator(-45, GetActorRotation().Yaw, GetActorRotation().Roll));

	// Update camera
	Camera->SetRelativeLocation(Transform->GetComponentLocation());
	Controller->SetControlRotation(GetActorRotation());
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

