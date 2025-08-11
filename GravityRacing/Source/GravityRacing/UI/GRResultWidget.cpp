// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GRResultWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/GRGameInstance.h"

void UGRResultWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (auto* GI = Cast<UGRGameInstance>(GetGameInstance()))
    {
        if (PlayerTime) PlayerTime->SetText(FText::FromString(GI->GetPlayTime()));
        if (Score)      Score->SetText(FText::AsNumber(GI->GetScore()));
    }

	if (ReplayButton)
		ReplayButton->OnClicked.AddDynamic(this, &UGRResultWidget::OnReplayClicked);

	if (ExitButton)
		ExitButton->OnClicked.AddDynamic(this, &UGRResultWidget::OnExitClicked);
}

void UGRResultWidget::OnReplayClicked()
{
	UGameplayStatics::OpenLevel(this, FName("LobbyLevel"));
}

void UGRResultWidget::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
