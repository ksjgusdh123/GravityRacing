// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GPGameWidget.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Framework/GRGameInstance.h"
#include "Kismet/GameplayStatics.h"

static FString FormatMMSSms(double Seconds)
{
	const int32 TotalCentis = static_cast<int32>(Seconds * 100.0 + 0.5);
	const int32 Min = TotalCentis / (60 * 100);
	const int32 Sec = (TotalCentis / 100) % 60;
	const int32 Cs = TotalCentis % 100;
	return FString::Printf(TEXT("%02d:%02d.%02d"), Min, Sec, Cs);
}

void UGPGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RefreshTimeLabel(AccumulatedSeconds);
	if (auto* GI = Cast<UGRGameInstance>(GetGameInstance()))
	{
		GI->OnScoreChanged.AddDynamic(this, &UGPGameWidget::SetScoreText);
	}
}

void UGPGameWidget::NativeDestruct()
{
	if (UWorld* W = GetWorld())
		W->GetTimerManager().ClearTimer(UiUpdateHandle);
	Super::NativeDestruct();
}

void UGPGameWidget::SetScoreText(int32 InScore)
{
	if (Score)
	{
		Score->SetText(FText::AsNumber(InScore));
	}
}

void UGPGameWidget::SetPlayerNameText(FString Name)
{
	if (PlayerName)
	{
		PlayerName->SetText(FText::FromString(Name));
	}
}

void UGPGameWidget::StartPlayTimer()
{
	if (!GetWorld()) return;

	AccumulatedSeconds = 0.0;
	StartWorldSeconds = GetWorld()->GetTimeSeconds();
	bTimerRunning = true;

	GetWorld()->GetTimerManager().SetTimer(
		UiUpdateHandle,
		[this]()
		{
			RefreshTimeLabel(GetElapsedPlaySeconds());
		},
		0.05f, true
	);
}

void UGPGameWidget::PausePlayTimer()
{
	if (!bTimerRunning || !GetWorld()) return;

	const double Now = GetWorld()->GetTimeSeconds();
	AccumulatedSeconds += (Now - StartWorldSeconds);
	bTimerRunning = false;
	RefreshTimeLabel(AccumulatedSeconds);
}

void UGPGameWidget::ResumePlayTimer()
{
	if (bTimerRunning || !GetWorld()) return;

	StartWorldSeconds = GetWorld()->GetTimeSeconds();
	bTimerRunning = true;
}

float UGPGameWidget::StopPlayTimerAndGetSeconds()
{
	if (GetWorld())
	{
		if (bTimerRunning)
		{
			const double Now = GetWorld()->GetTimeSeconds();
			AccumulatedSeconds += (Now - StartWorldSeconds);
			bTimerRunning = false;
		}
		GetWorld()->GetTimerManager().ClearTimer(UiUpdateHandle);
	}

	RefreshTimeLabel(AccumulatedSeconds);
	return static_cast<float>(AccumulatedSeconds);
}

float UGPGameWidget::GetElapsedPlaySeconds() const
{
	if (!GetWorld()) return static_cast<float>(AccumulatedSeconds);

	if (bTimerRunning)
	{
		const double Now = GetWorld()->GetTimeSeconds();
		return static_cast<float>(AccumulatedSeconds + (Now - StartWorldSeconds));
	}
	return static_cast<float>(AccumulatedSeconds);
}

void UGPGameWidget::RefreshTimeLabel(float Seconds)
{
	if (PlayerTime)
	{
		PlayerTime->SetText(FText::FromString(FormatMMSSms(Seconds)));
	}
}
