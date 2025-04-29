// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "StrucFish.h"
#include "PlayerHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOAT_API UPlayerHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UPlayerHudWidget(const FObjectInitializer& ObjectInitializer);

	// Constructs the widget
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) UTextBlock* CaughtTextBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float CaughtTextWaitTime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float CaughtTextFadeTime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) UCanvasPanelSlot* FishIconSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) UCanvasPanel* GaugueCanvas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float FishPosMax = 180.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float FishPosMin = -180.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float FishPosStart = -10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes) float FishStrengthMultiplier = 50.0f; 
	float FishPosCurrent;
	float CaughtTextTime = 0;
	FStrucFish CurrentFish;
	int FishDirection = 1;

	// Updates the UI
	virtual void UpdateUI(float DeltaTime);

	// Shows and then fades out the caught text
	virtual void ShowCaughtText(FText Text);

	// Starts the fish gauge minigame
	virtual void StartFishGauge(FStrucFish FishType);

	// Adds to the fish gauge position in the input direction
	virtual void AddToGauge(float InputValue);
	
};
