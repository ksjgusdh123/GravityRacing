// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GRGameMode.h"
#include "Map/GRTunnel.h"
#include "Map/GRMapGenerator.h"
#include "Characters/Player/GRPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "System/GRUISystem.h"

void AGRGameMode::BeginPlay()
{
	Super::BeginPlay();

	AGRPlayer* Player = Cast<AGRPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		Player->SetRoadOneLineDistance(AGRMapGenerator::OneLineLengthY);
	}

	UGRUISystem* UISys = GetGameInstance()->GetSubsystem<UGRUISystem>();
	if (UISys)
	{
		UISys->OnGame();
	}
}


void AGRGameMode::AddScore(int32 Amount)
{
	Score += Amount;
	OnScoreChanged.Broadcast(Score);
}