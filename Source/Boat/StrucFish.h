// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "StrucFish.generated.h"

USTRUCT(BlueprintType)
struct FStrucFish
{
	GENERATED_USTRUCT_BODY()

	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Name = "";
		UPROPERTY(EditAnywhere, BlueprintReadWrite) float Strength = 1;
		UPROPERTY(EditAnywhere, BlueprintReadWrite) float Rarity = 1;
		UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMesh* Mesh;
		UPROPERTY(EditAnywhere, BlueprintReadWrite) UMaterialInterface* Material;
};

