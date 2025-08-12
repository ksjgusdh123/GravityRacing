// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GRLobbyWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGRLobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UGRLobbyWidget::OnStartButtonClicked);
    }
}

void UGRLobbyWidget::OnStartButtonClicked()
{
    UGameplayStatics::OpenLevel(this, FName("MainLevel"));
}