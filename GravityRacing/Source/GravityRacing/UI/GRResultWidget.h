// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GRResultWidget.generated.h"

class UButton;

UCLASS()
class GRAVITYRACING_API UGRResultWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnReplayClicked();

	UFUNCTION()
	void OnExitClicked();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UTextBlock* PlayerTime;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UTextBlock* Score;

	UPROPERTY(meta = (BindWidget))
	UButton* ReplayButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
};
