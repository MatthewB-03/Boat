// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraActor.h"
#include "PlayerRod.h"
#include "math.h"
#include "PlayerPawn.generated.h"

UCLASS()
class BOAT_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float MoveSpeed = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float TurnSpeed = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float CollisionRadius = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float CollisionYOffset = -100;
	float MoveForward;
	float MoveRight;
	float LookUp;
	float LookRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) USceneComponent* Transform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) USceneComponent* RodTransformPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) UChildActorComponent* RodChildActor;
	APlayerRod* RodActor;
	FCollisionQueryParams CollisionParams;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
