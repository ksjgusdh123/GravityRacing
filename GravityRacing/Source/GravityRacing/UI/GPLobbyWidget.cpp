// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GPLobbyWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/GRGameInstance.h"

void UGPLobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (NameEditText)
    {
        NameEditText->OnTextCommitted.AddDynamic(this, &UGPLobbyWidget::OnNameCommitted);
    }

    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UGPLobbyWidget::OnStartButtonClicked);
    }
}

void UGPLobbyWidget::OnNameCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    if (auto* GI = Cast<UGRGameInstance>(GetGameInstance()))
    {
        GI->SetPlayerName(Text.ToString());
    }
}

void UGPLobbyWidget::OnStartButtonClicked()
{
    UGameplayStatics::OpenLevel(this, FName("MainLevel"));
}