// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "SplineMeshRenderer.generated.h"

UCLASS()
class BOAT_API ASplineMeshRenderer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineMeshRenderer();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) UStaticMesh* SplineBaseMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) UMaterialInterface* SplineMeshMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) int NumberOfPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) int PointSpacing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) int WaveLengthMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) int WaveAmplitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float WaveSpeed;
	float WaveOffsetValue;
	USplineComponent* Spline;
	TArray<USplineMeshComponent*> SplineMeshes;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Draws a spline mesh around the current spline
	virtual void DrawSplineMesh();

};
