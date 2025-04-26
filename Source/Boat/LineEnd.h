// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) UStaticMesh* HookModel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) UStaticMeshComponent* Mesh;

	FVector Velocity;
	bool Falling = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Casts the line in the given direction
	virtual void Cast(FVector Direction);

};
