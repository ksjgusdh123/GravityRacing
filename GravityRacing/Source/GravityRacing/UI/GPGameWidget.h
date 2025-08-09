// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GPGameWidget.generated.h"

UCLASS()
class GRAVITYRACING_API UGPGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void SetScoreText(int32 Score);
	UFUNCTION()
	void SetPlayerNameText(FString Name);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UTextBlock* PlayerTime;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UTextBlock* Score;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UTextBlock* PlayerName;

	UFUNCTION(BlueprintCallable, Category = "GR|GameWidget|Timer")
	void StartPlayTimer();

	UFUNCTION(BlueprintCallable, Category = "GR|GameWidget|Timer")
	void PausePlayTimer();

	UFUNCTION(BlueprintCallable, Category = "GR|GameWidget|Timer")
	void ResumePlayTimer();

	UFUNCTION(BlueprintCallable, Category = "GR|GameWidget|Timer")
	float StopPlayTimerAndGetSeconds();

	UFUNCTION(BlueprintCallable, Category = "GR|GameWidget|Timer")
	float GetElapsedPlaySeconds() const;

private:
	void RefreshTimeLabel(float Seconds);

private:
	double AccumulatedSeconds = 0.0;
	double StartWorldSeconds = 0.0;
	bool bTimerRunning = false;

	FTimerHandle UiUpdateHandle;
};
