// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GRGameWidget.h"
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

void UGRGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RefreshTimeLabel(AccumulatedSeconds);
	if (auto* GI = Cast<UGRGameInstance>(GetGameInstance()))
	{
		GI->OnScoreChanged.AddDynamic(this, &UGRGameWidget::SetScoreText);
	}
}

void UGRGameWidget::NativeDestruct()
{
	if (UWorld* W = GetWorld())
		W->GetTimerManager().ClearTimer(UiUpdateHandle);
	Super::NativeDestruct();
}

void UGRGameWidget::SetScoreText(int32 InScore)
{
	if (Score)
	{
		Score->SetText(FText::AsNumber(InScore));
	}
}

void UGRGameWidget::StartPlayTimer()
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

void UGRGameWidget::PausePlayTimer()
{
	if (!bTimerRunning || !GetWorld()) return;

	const double Now = GetWorld()->GetTimeSeconds();
	AccumulatedSeconds += (Now - StartWorldSeconds);
	bTimerRunning = false;
	RefreshTimeLabel(AccumulatedSeconds);
}

void UGRGameWidget::ResumePlayTimer()
{
	if (bTimerRunning || !GetWorld()) return;

	StartWorldSeconds = GetWorld()->GetTimeSeconds();
	bTimerRunning = true;
}

float UGRGameWidget::StopPlayTimerAndGetSeconds()
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

float UGRGameWidget::GetElapsedPlaySeconds() const
{
	if (!GetWorld()) return static_cast<float>(AccumulatedSeconds);

	if (bTimerRunning)
	{
		const double Now = GetWorld()->GetTimeSeconds();
		return static_cast<float>(AccumulatedSeconds + (Now - StartWorldSeconds));
	}
	return static_cast<float>(AccumulatedSeconds);
}

void UGRGameWidget::RefreshTimeLabel(float Seconds)
{
	if (PlayerTime)
	{
		FString FormatTime = FormatMMSSms(Seconds);
		PlayerTime->SetText(FText::FromString(FormatTime));
		auto* GI = Cast<UGRGameInstance>(GetGameInstance());
		if (GI)
		{
			GI->SetPlayTime(FormatTime);
		}
	}
}
