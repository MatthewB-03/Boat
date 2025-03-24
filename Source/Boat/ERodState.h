// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "ERodState.generated.h"

UENUM()
enum RodState
{
	Idle UMETA(DisplayName = "Idle"),
	Drawing UMETA(DisplayName = "Drawing"),
	Casting UMETA(DisplayName = "Casting"),
	Reeling UMETA(DisplayName = "Reeling"),
	Caught UMETA(DisplayName = "Caught")
};


/**
 * 
 */
UCLASS()
class BOAT_API UERodState : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};