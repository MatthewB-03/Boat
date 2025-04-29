// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHudWidget.h"

// Sets default values
UPlayerHudWidget::UPlayerHudWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

// Constructs the widget
void UPlayerHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Start caught text fully transparent
	CaughtTextBox->SetColorAndOpacity(FSlateColor(FLinearColor(1, 1, 1, 0)));

	// Hide gaugue
	GaugueCanvas->SetVisibility(ESlateVisibility::Hidden);
}

// Updates the UI
void UPlayerHudWidget::UpdateUI(float DeltaTime)
{
	// Wait and fade out text
	if (CaughtTextTime > 0) 
	{
		CaughtTextTime -= DeltaTime;

		// If done fading out
		if (CaughtTextTime <= 0)
		{
			// Set as fully transparent
			CaughtTextBox->SetColorAndOpacity(FSlateColor(FLinearColor(1, 1, 1, 0)));
		}
		// If done waiting
		else if (CaughtTextTime < CaughtTextFadeTime) 
		{
			// Fade out
			CaughtTextBox->SetColorAndOpacity(FSlateColor(FLinearColor(1, 1, 1, CaughtTextTime / CaughtTextFadeTime)));
		}
	}
	
	// If gauge minigame showing
	if (GaugueCanvas->GetVisibility() == ESlateVisibility::Visible) 
	{
		// Random chance to swap direction
		if (FMath::RandRange(0.0f, 100.0f) <= CurrentFish.PercentSwapDirection) 
		{
			FishDirection *= -1;
		}

		// Fish pulling
		AddToGauge(FishDirection * DeltaTime * 1.1f);

		// Fail minigame if past gauge threshold
		if (FishPosCurrent >= FishPosMax || FishPosCurrent <= FishPosMin)
		{
			GaugueCanvas->SetVisibility(ESlateVisibility::Hidden);
		}

		// Update icon position
		FishIconSlot->SetPosition(FVector2D(FishIconSlot->GetPosition().X, FishPosCurrent));
	}
}

// Shows and then fades out the caught text
void UPlayerHudWidget::ShowCaughtText(FText Text)
{
	CaughtTextTime = CaughtTextFadeTime + CaughtTextWaitTime;
	CaughtTextBox->SetText(Text);
	CaughtTextBox->SetColorAndOpacity(FSlateColor(FLinearColor(1, 1, 1, 1)));
}

// Starts the fish gauge minigame
void UPlayerHudWidget::StartFishGauge(FStrucFish FishType)
{
	FishPosCurrent = FishPosStart;
	GaugueCanvas->SetVisibility(ESlateVisibility::Visible);
}

// Adds to the fish gauge position in the input direction
void UPlayerHudWidget::AddToGauge(float InputValue) 
{
	FishPosCurrent += InputValue * FishStrengthMultiplier * CurrentFish.Strength;
}