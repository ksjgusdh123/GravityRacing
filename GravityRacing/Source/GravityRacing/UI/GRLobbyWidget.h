// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GRLobbyWidget.generated.h"

class UEditableTextBox;
class UButton;

UCLASS()
class GRAVITYRACING_API UGRLobbyWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UButton* StartButton;

private:
    UFUNCTION()
    void OnStartButtonClicked();
};