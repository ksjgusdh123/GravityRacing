// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GRLobbyWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/GRGameInstance.h"

void UGRLobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (NameEditText)
    {
        NameEditText->OnTextCommitted.AddDynamic(this, &UGRLobbyWidget::OnNameCommitted);
    }

    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UGRLobbyWidget::OnStartButtonClicked);
    }
}

void UGRLobbyWidget::OnNameCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    if (auto* GI = Cast<UGRGameInstance>(GetGameInstance()))
    {
        GI->SetPlayerName(Text.ToString());
    }
}

void UGRLobbyWidget::OnStartButtonClicked()
{
    UGameplayStatics::OpenLevel(this, FName("MainLevel"));
}