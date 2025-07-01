// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Objects/Coin/GRCoin.h"
#include "GRCoinSpawner.generated.h"

UCLASS()
class GRAVITYRACING_API AGRCoinSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AGRCoinSpawner();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Coin")
	void SpawnCoinsInLength(float Length, int32 MaxCoins);

private:
	UPROPERTY(EditAnywhere, Category = "Coin")
	TSubclassOf<AGRCoin> CoinClass;
};
