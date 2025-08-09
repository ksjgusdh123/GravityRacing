// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/GRPauseWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGRPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayButton)
		PlayButton->OnClicked.AddDynamic(this, &UGRPauseWidget::OnPlayClicked);

	if (ReplayButton)
		ReplayButton->OnClicked.AddDynamic(this, &UGRPauseWidget::OnReplayClicked);

	if (ExitButton)
		ExitButton->OnClicked.AddDynamic(this, &UGRPauseWidget::OnExitClicked);
}

void UGRPauseWidget::OnPlayClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Play Button Clicked"));
}

void UGRPauseWidget::OnReplayClicked()
{
	UGameplayStatics::OpenLevel(this, FName("LobbyLevel"));
}

void UGRPauseWidget::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
