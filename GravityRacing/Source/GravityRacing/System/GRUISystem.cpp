// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GRUISystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/GRLobbyWidget.h"
#include "UI/GRGameWidget.h"
#include "UI/GRPauseWidget.h"
#include "UI/GRResultWidget.h"
#include "System/GRSoundSystem.h"

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
	static ConstructorHelpers::FClassFinder<UGRPauseWidget> PauseBP(TEXT("/Game/GravityRacing/UI/Widgets/WBP_Pause"));
	if (PauseBP.Succeeded())
		PauseWidgetClass = PauseBP.Class;
	static ConstructorHelpers::FClassFinder<UGRResultWidget> ResultBP(TEXT("/Game/GravityRacing/UI/Widgets/WBP_Result"));
	if (ResultBP.Succeeded())
		ResultWidgetClass = ResultBP.Class;
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

void UGRUISystem::SetGameAndUIMode(UUserWidget* FocusWidget)
{
	UE_LOG(LogTemp, Log, TEXT("SetGameMode"));
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) { UE_LOG(LogTemp, Error, TEXT("Invalid PlayerController")); return; }

	FInputModeGameAndUI InputMode;

	if (FocusWidget)
	{
		FocusWidget->SetIsFocusable(true);
		InputMode.SetWidgetToFocus(FocusWidget->TakeWidget());
	}

	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);

	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;
}

void UGRUISystem::OnLobby()
{
	UE_LOG(LogTemp, Log, TEXT("OnLobby"));
	UGRSoundSystem* SoundSys = GetGameInstance()->GetSubsystem<UGRSoundSystem>();
	if (SoundSys)
	{
		SoundSys->PlayBGM(EGameSound::LobbyBGM);
	}

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

	UGRSoundSystem* SoundSys = GetGameInstance()->GetSubsystem<UGRSoundSystem>();
	if (SoundSys)
	{
		SoundSys->PlayBGM(EGameSound::MainBGM);
	}

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
		SetGameAndUIMode(GameWidget);
		UE_LOG(LogTemp, Log, TEXT("GameWidget Added to Viewport"));

		GameWidget->StartPlayTimer();
	}
}

void UGRUISystem::OnResult()
{
	UGRSoundSystem* SoundSys = GetGameInstance()->GetSubsystem<UGRSoundSystem>();
	if (SoundSys)
	{
		SoundSys->StopBGM();
		SoundSys->Play2D(EGameSound::Die);
	}

	UE_LOG(LogTemp, Log, TEXT("OnResult"));

	if (GameWidget)
	{
		GameWidget->RemoveFromParent();
		GameWidget = nullptr;
		UE_LOG(LogTemp, Log, TEXT("Old GameWidget Removed"));
	}

	if (ResultWidget)
	{
		ResultWidget->RemoveFromParent();
		ResultWidget = nullptr;
		UE_LOG(LogTemp, Log, TEXT("Old ResultWidget Removed"));
	}

	if (ResultWidgetClass)
	{
		ResultWidget = CreateWidget<UGRResultWidget>(GetWorld(), ResultWidgetClass);
		UE_LOG(LogTemp, Log, TEXT("Created ResultWidget"));
	}

	if (ResultWidget && !ResultWidget->IsInViewport())
	{
		ResultWidget->AddToViewport();
		SetUIMode(true, ResultWidget);
		UE_LOG(LogTemp, Log, TEXT("ResultWidget Added to Viewport"));
	}
}


void UGRUISystem::ShowPause()
{
	UE_LOG(LogTemp, Log, TEXT("OnPause"));
	UGRSoundSystem* SoundSys = GetGameInstance()->GetSubsystem<UGRSoundSystem>();
	if (SoundSys)
	{
		SoundSys->StopBGM();
	}

	if (PauseWidget)
	{
		PauseWidget->RemoveFromParent();
		PauseWidget = nullptr;
		UE_LOG(LogTemp, Log, TEXT("Old PauseWidget Removed"));
	}

	if (PauseWidgetClass)
	{
		PauseWidget = CreateWidget<UGRPauseWidget>(GetWorld(), PauseWidgetClass);
		UE_LOG(LogTemp, Log, TEXT("Created PauseWidget"));
	}

	if (PauseWidget && !PauseWidget->IsInViewport())
	{
		PauseWidget->AddToViewport();
		SetUIMode(true, PauseWidget);
		UE_LOG(LogTemp, Log, TEXT("PauseWidget Added to Viewport"));
	}
}

void UGRUISystem::HidePause()
{
	UGRSoundSystem* SoundSys = GetGameInstance()->GetSubsystem<UGRSoundSystem>();
	if (SoundSys)
	{
		SoundSys->PlayBGM(EGameSound::MainBGM);
	}

	if (PauseWidget)
	{
		PauseWidget->RemoveFromParent();
		PauseWidget = nullptr;
		UE_LOG(LogTemp, Log, TEXT("PauseWidget Removed"));
	}

}
