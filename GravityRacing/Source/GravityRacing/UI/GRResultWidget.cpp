// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GRResultWidget.h"
#include "Framework/GRGameInstance.h"

void UGRResultWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (auto* GI = Cast<UGRGameInstance>(GetGameInstance()))
    {
        if (PlayerTime) PlayerTime->SetText(FText::FromString(GI->GetPlayTime()));
        if (Score)      Score->SetText(FText::AsNumber(GI->GetScore()));
    }
}