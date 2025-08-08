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
    UFUNCTION(BlueprintCallable, Category = "Coin|Spawn")
    void SpawnCoinsInLength(float Length, int32 MaxCoins);

    UPROPERTY(EditAnywhere, Category = "Coin|Variety")
    TSubclassOf<AGRCoin> CoinClass;

    UPROPERTY(EditAnywhere, Category = "Coin|Variety")
    TSubclassOf<AGRCoin> BigCoinClass;

    UPROPERTY(EditAnywhere, Category = "Coin|Variety")
    TSubclassOf<AGRCoin> MagnetCoinClass;

    UPROPERTY(EditAnywhere, Category = "Coin|Variety", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float BigCoinChance = 0.10f;

    UPROPERTY(EditAnywhere, Category = "Coin|Variety", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MagnetCoinChance = 0.05f;

    UPROPERTY(EditAnywhere, Category = "Coin|Variety")
    int32 NormalValue = 10;

    UPROPERTY(EditAnywhere, Category = "Coin|Variety")
    int32 BigValue = 50;

    UPROPERTY(EditAnywhere, Category = "Coin|Spawn")
    float OverlapCheckRadius = 40.f;

    UPROPERTY(EditAnywhere, Category = "Coin|Spawn")
    TEnumAsByte<ECollisionChannel> OverlapCheckChannel = ECC_WorldStatic;

private:
    bool CanSpawnAt(const FVector& Loc, float Radius) const;
    TSubclassOf<AGRCoin> PickCoinClassAndValue(int32& OutValue) const;
    bool TrySpawnOne(const FVector& Loc, const FRotator& Rot, int32& OutSpawned);
};