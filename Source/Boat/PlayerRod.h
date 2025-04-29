// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "RodAnimInstance.h"
#include "ERodState.h"
#include "EHookState.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "LineEnd.h"
#include "StrucFish.h"
#include "PlayerHudWidget.h"
#include "Misc/DateTime.h"
#include "iostream"
#include "chrono"
#include "ctime"
#include "PlayerRod.generated.h"

UCLASS()
class BOAT_API APlayerRod : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerRod();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) UStaticMesh* SplineBaseMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) UMaterialInterface* SplineMeshMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float DrawSpeed = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float StartReelingSpeed = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float StopReelingSpeed = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) UChildActorComponent* LineEndChildActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) TSubclassOf<UPlayerHudWidget> PlayerHudType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) TArray<FStrucFish> FishTypes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float CaughtFishZOffset = -20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float LineRopeDip = -20;
	FStrucFish FishType;
	UPlayerHudWidget* PlayerHud;
	ALineEnd* LineEndActor;
	USkeletalMeshComponent* Mesh;
	USplineComponent* Spline;
	TArray<USplineMeshComponent*> SplineMeshes;
	URodAnimInstance* AnimInstance;
	bool MouseIsDown;
	RodState CurrentState = RodState::Idle;
	float StateTime = 0;
	float FishWaitTime = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called every frame in the idle state
	virtual void IdleTick(float DeltaTime);

	// Called every frame in the drawing state
	virtual void DrawingTick(float DeltaTime);

	// Called every frame in the casting state
	virtual void CastingTick(float DeltaTime);

	// Called every frame in the reeling state
	virtual void ReelingTick(float DeltaTime);

	// Called every frame in the caught state
	virtual void CaughtTick(float DeltaTime);

	// Called when the mouse is clicked down
	virtual void MouseDown();

	// Called when the mouse click is released
	virtual void MouseUp();

	// Gets a random fish type from the array
	virtual FStrucFish GetRandomFish();

	// Draws a spline mesh around the current spline
	virtual void DrawSplineMesh();

	// Creates a straight spline
	virtual void CreateSplineStraight();

	// Creates a curved spline
	virtual void CreateSplineCurved();

};
