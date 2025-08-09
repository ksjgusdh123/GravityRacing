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

protected:
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* NameEditText;

    UPROPERTY(meta = (BindWidget))
    UButton* StartButton;

private:
    UFUNCTION()
    void OnNameCommitted(const FText& Text, ETextCommit::Type CommitMethod);

    UFUNCTION()
    void OnStartButtonClicked();
};