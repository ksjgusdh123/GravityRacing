// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GRUISystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/GRLobbyWidget.h"
#include "UI/GRGameWidget.h"

UGRUISystem::UGRUISystem()
{
	LoadWidgetClasses();
}

void UGRUISystem::LoadWidgetClasses()
{
	static ConstructorHelpers::FClassFinder<UGRLobbyWidget> LobbyBP(TEXT("/Game/GravityRacing/UI/Widgets/WBP_Lobby"));
	if (LobbyBP.Succeeded())
		LobbyWidgetClass = LobbyBP.Class;
	static ConstructorHelpers::FClassFinder<UGRGameWidget> GameBP(TEXT("/Game/GravityRacing/UI/Widgets/WBP_Game"));
	if (GameBP.Succeeded())
		GameWidgetClass = GameBP.Class;
}

void UGRUISystem::SetUIMode(bool bShowCursor, UUserWidget* FocusWidget)
{
	UE_LOG(LogTemp, Log, TEXT("SetUIMode: bShowCursor=%s, FocusWidget=%s"),
		bShowCursor ? TEXT("true") : TEXT("false"),
		FocusWidget ? *FocusWidget->GetName() : TEXT("NULL"));

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) { UE_LOG(LogTemp, Error, TEXT("Invalid PlayerController")); return; }

	FInputModeUIOnly InputMode;
	if (FocusWidget && FocusWidget->IsValidLowLevel())
	{
		FocusWidget->SetIsFocusable(true);
		InputMode.SetWidgetToFocus(FocusWidget->TakeWidget());
	}
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = bShowCursor;

	PC->SetIgnoreMoveInput(true);
	PC->SetIgnoreLookInput(true);

	if (APawn* Pawn = PC->GetPawn())
		if (Pawn->Controller == PC) Pawn->DisableInput(PC);
}

void UGRUISystem::SetGameMode()
{
	UE_LOG(LogTemp, Log, TEXT("SetGameMode"));
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) { UE_LOG(LogTemp, Error, TEXT("Invalid PlayerController")); return; }

	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = false;

	PC->SetIgnoreMoveInput(false);
	PC->SetIgnoreLookInput(false);

	if (APawn* Pawn = PC->GetPawn())
		if (Pawn->Controller == PC) Pawn->EnableInput(PC);
}

void UGRUISystem::OnLobby()
{
	UE_LOG(LogTemp, Log, TEXT("OnLobby"));

	if (LobbyWidget)
	{
		LobbyWidget->RemoveFromParent();
		LobbyWidget = nullptr;
		UE_LOG(LogTemp, Log, TEXT("Old LobbyWidget Removed"));
	}

	if (LobbyWidgetClass)
	{
		LobbyWidget = CreateWidget<UGRLobbyWidget>(GetWorld(), LobbyWidgetClass);
		UE_LOG(LogTemp, Log, TEXT("Created LobbyWidget"));
	}

	if (LobbyWidget && !LobbyWidget->IsInViewport())
	{
		LobbyWidget->AddToViewport();
		SetUIMode(true, LobbyWidget);
		UE_LOG(LogTemp, Log, TEXT("LobbyWidget Added to Viewport"));
	}
}

void UGRUISystem::OnGame()
{
	UE_LOG(LogTemp, Log, TEXT("OnGame"));

	if (GameWidget)
	{
		GameWidget->RemoveFromParent();
		GameWidget = nullptr;
		UE_LOG(LogTemp, Log, TEXT("Old GameWidget Removed"));
	}

	if (GameWidgetClass)
	{
		GameWidget = CreateWidget<UGRGameWidget>(GetWorld(), GameWidgetClass);
		UE_LOG(LogTemp, Log, TEXT("Created GameWidget"));
	}

	if (GameWidget && !GameWidget->IsInViewport())
	{
		GameWidget->AddToViewport();
		SetGameMode();
		UE_LOG(LogTemp, Log, TEXT("GameWidget Added to Viewport"));

		GameWidget->StartPlayTimer();
	}
}
