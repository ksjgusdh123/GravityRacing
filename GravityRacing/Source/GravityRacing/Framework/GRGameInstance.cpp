// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/GRGameInstance.h"

void UGRGameInstance::Init()
{
	Super::Init();
}

void UGRGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UGRGameInstance::OnStart()
{
	Super::OnStart();
}

void UGRGameInstance::AddScore(int32 Amount)
{
	Score += Amount;
	OnScoreChanged.Broadcast(Score);
}

void UGRGameInstance::SetPlayerName(const FString& NewName)
{
	PlayerName = NewName;
	OnPlayerNameChanged.Broadcast(PlayerName);
}