// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Coin/GRCoinSpawner.h"
#include "GravityRacing.h"
#include "Engine/World.h"

AGRCoinSpawner::AGRCoinSpawner()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGRCoinSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AGRCoinSpawner::SpawnCoinsInLength(float Length, int32 MaxCoins)
{
	if (!CoinClass)
	{
		GRLOG_E("CoinClass is null");
		return;
	}

	const float StartX = GetActorLocation().X;
	const float Y = GetActorLocation().Y;
	const float Z = GetActorLocation().Z + 100.f;

	const float Spacing = Length / MaxCoins;
	int32 SpawnCnt = 0;
	for (int32 i = 0; i < MaxCoins; ++i)
	{
		float SpawnX = StartX + i * Spacing;
		FVector SpawnLocation(SpawnX, Y, Z);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters Params;
		auto* Coin = GetWorld()->SpawnActor<AGRCoin>(CoinClass, SpawnLocation, SpawnRotation, Params);
		if (Coin)
		{
			SpawnCnt++;
		}
	}
	GRLOG("Spawn Count : %d", SpawnCnt);
}
