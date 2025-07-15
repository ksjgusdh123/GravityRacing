// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GRGameModeBase.h"
#include "Map/GRTunnel.h"
#include "Map/GRMapGenerator.h"
#include "Characters/Player/GRPlayer.h"
#include "Kismet/GameplayStatics.h"

void AGRGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	AGRPlayer* Player = Cast<AGRPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		Player->SetRoadOneLineDistance(AGRMapGenerator::OneLineLengthY);
	}
}

void AGRGameModeBase::AddScore(int32 Amount)
{
	Score += Amount;
	OnScoreChanged.Broadcast(Score);
}
