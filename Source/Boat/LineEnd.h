// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Components/StaticMeshComponent.h"
#include "EHookState.h"
#include "math.h"
#include "LineEnd.generated.h"

UCLASS()
class BOAT_API ALineEnd : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALineEnd();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float SeaHeight = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float CastSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float FallAcceleration = 9.81f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float CollisionRadius = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) UStaticMesh* HookModel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) UMaterialInterface* HookMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) UStaticMeshComponent* Mesh;
	FVector Velocity;
	TEnumAsByte<HookState> State = HookState::Idle;
	FCollisionQueryParams CollisionParams;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Casts the line in the given direction
	virtual void Cast(FVector Direction);

	// Sets the line end to a new model
	virtual void SetModel(UStaticMesh* NewMesh, UMaterialInterface* NewMaterial);

	// Resets to the hook model
	virtual void ResetModel();

};
