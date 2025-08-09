// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GPUISystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/GPGameWidget.h"

UGPUISystem::UGPUISystem()
{
	LoadWidgetClasses();
}

void UGPUISystem::LoadWidgetClasses()
{
	static ConstructorHelpers::FClassFinder<UGPGameWidget> BP(TEXT("/Game/GravityRacing/UI/Widgets/WBP_Game"));
	if (BP.Succeeded())
		GameWidgetClass = BP.Class;
}

void UGPUISystem::SetUIMode(bool bShowCursor, UUserWidget* FocusWidget)
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

void UGPUISystem::SetGameMode()
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

void UGPUISystem::OnGameStart()
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
		GameWidget = CreateWidget<UGPGameWidget>(GetWorld(), GameWidgetClass);
		UE_LOG(LogTemp, Log, TEXT("Created GameWidget"));
	}

	if (GameWidget && !GameWidget->IsInViewport())
	{
		GameWidget->AddToViewport();
		UE_LOG(LogTemp, Log, TEXT("GameWidget Added to Viewport"));

		GameWidget->StartPlayTimer();
	}
}
