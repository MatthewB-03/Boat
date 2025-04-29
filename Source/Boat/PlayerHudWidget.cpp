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
}

// Updates the UI
void UPlayerHudWidget::UpdateUI(float DeltaTime)
{
	if (CaughtTextTime > 0) 
	{
		CaughtTextTime -= DeltaTime;
		if (CaughtTextTime <= 0)
		{
			CaughtTextBox->SetColorAndOpacity(FSlateColor(FLinearColor(1, 1, 1, 0)));
		}
		else if (CaughtTextTime < CaughtTextFadeTime) 
		{
			CaughtTextBox->SetColorAndOpacity(FSlateColor(FLinearColor(1, 1, 1, CaughtTextTime / CaughtTextFadeTime)));
		}
	}
}

// Shows and then fades out the caught text
void UPlayerHudWidget::ShowCaughtText(FText Text)
{
	CaughtTextTime = CaughtTextFadeTime + CaughtTextWaitTime;
	CaughtTextBox->SetText(Text);
	CaughtTextBox->SetColorAndOpacity(FSlateColor(FLinearColor(1, 1, 1, 1)));
}