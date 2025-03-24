// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Create components
	Transform = CreateAbstractDefaultSubobject<USceneComponent>("Scene", false);
	RodTransformPosition = CreateAbstractDefaultSubobject<USceneComponent>("RodPosition", false);
	Camera = CreateAbstractDefaultSubobject<UCameraComponent>("Camera", false);
	RodChildActor = CreateAbstractDefaultSubobject<UChildActorComponent>("RodChildActor", false);

	// Attach rod to transform position
	RodChildActor->SetupAttachment(RodTransformPosition);
	RodTransformPosition->SetupAttachment(Transform);

	// Set root transform
	RootComponent = Transform;

	// Auto possess player 0
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Define collision channel
	CollisionParams = FCollisionQueryParams();
	CollisionParams.bTraceComplex = true;

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

	// Get rod actor
	Rod = Cast<APlayerRod>(RodChildActor->GetChildActor());
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

	// Check for forwards collision
	FVector Movement = GetActorForwardVector() * MoveForward * MoveSpeed * DeltaTime * FVector(1.0f, 1.0f, 0.0f);
	FHitResult Hit;
	if (!GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation() + FVector(0.0f, 0.0f, CollisionYOffset), GetActorLocation() + FVector(0.0f, 0.0f, CollisionYOffset) + Movement.GetSafeNormal() * CollisionRadius, ECC_Pawn, CollisionParams))
	{
		// Move player forwards
		SetActorLocation(GetActorLocation() + Movement);
	}

	// Check for sideways collision
	Movement = GetActorRightVector() * MoveRight * MoveSpeed * DeltaTime * FVector(1.0f, 1.0f, 0.0f);
	if (!GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation() + FVector(0.0f, 0.0f, CollisionYOffset), GetActorLocation() + FVector(0.0f, 0.0f, CollisionYOffset) + Movement.GetSafeNormal() * CollisionRadius, ECC_Pawn, CollisionParams))
	{
		// Move player sideways
		SetActorLocation(GetActorLocation() + Movement);
	}

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

