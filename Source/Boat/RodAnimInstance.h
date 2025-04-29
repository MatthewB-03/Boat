// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ERodState.h"
#include "RodAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BOAT_API URodAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) RodState CurrentState = RodState::Idle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float BlendAlpha;
	
};
