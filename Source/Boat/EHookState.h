// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EHookState.generated.h"

UENUM()
enum class HookState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Falling = 1 UMETA(DisplayName = "Falling"),
	InWater = 2 UMETA(DisplayName = "InWater")
};

/**
 * 
 */
UCLASS()
class BOAT_API UEHookState : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
