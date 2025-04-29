// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
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
	float CaughtTextTime = 0;

	// Updates the UI
	virtual void UpdateUI(float DeltaTime);

	// Shows and then fades out the caught text
	virtual void ShowCaughtText(FText Text);
	
};
