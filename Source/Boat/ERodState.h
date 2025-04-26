// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "ERodState.generated.h"

UENUM()
enum class RodState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Drawing = 1 UMETA(DisplayName = "Drawing"),
	Casting = 2 UMETA(DisplayName = "Casting"),
	Reeling = 3 UMETA(DisplayName = "Reeling"),
	Caught = 4 UMETA(DisplayName = "Caught")
};


/**
 * 
 */
UCLASS()
class BOAT_API UERodState : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};