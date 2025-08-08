#include "Objects/Coin/GRCoinSpawner.h"
#include "GravityRacing.h"
#include "Objects/Coin/GRCoin.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AGRCoinSpawner::AGRCoinSpawner()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGRCoinSpawner::BeginPlay()
{
    Super::BeginPlay();
}

bool AGRCoinSpawner::CanSpawnAt(const FVector& Loc, float Radius) const
{
    FCollisionQueryParams Params(SCENE_QUERY_STAT(GRCoinSpawn), /*bTraceComplex*/ false, this);
    FHitResult Hit;
    const bool bHit = GetWorld()->SweepSingleByChannel(
        Hit, Loc, Loc, FQuat::Identity,
        OverlapCheckChannel,
        FCollisionShape::MakeSphere(Radius),
        Params
    );
    return !bHit;
}

TSubclassOf<AGRCoin> AGRCoinSpawner::PickCoinClassAndValue(int32& OutValue) const
{
    float r = FMath::FRand();

    if (MagnetCoinClass && r < MagnetCoinChance)
    {
        OutValue = NormalValue;
        return MagnetCoinClass;
    }
    r -= MagnetCoinChance;

    if (BigCoinClass && r < BigCoinChance)
    {
        OutValue = BigValue;
        return BigCoinClass;
    }

    OutValue = NormalValue;
    return CoinClass;
}

bool AGRCoinSpawner::TrySpawnOne(const FVector& Loc, const FRotator& Rot, int32& OutSpawned)
{
    int32 ValueToSet = NormalValue;
    TSubclassOf<AGRCoin> ClassToSpawn = PickCoinClassAndValue(ValueToSet);
    if (!ClassToSpawn) return false;

    if (!CanSpawnAt(Loc, OverlapCheckRadius))
        return false;

    FTransform Xform(Rot, Loc);
    AGRCoin* Coin = GetWorld()->SpawnActorDeferred<AGRCoin>(ClassToSpawn, Xform, this, nullptr,
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    if (!Coin) return false;

    Coin->Value = ValueToSet;
    UGameplayStatics::FinishSpawningActor(Coin, Xform);

    ++OutSpawned;
    return true;
}

void AGRCoinSpawner::SpawnCoinsInLength(float Length, int32 MaxCoins)
{
    if (!CoinClass)
    {
        GRLOG_E("CoinClass is null");
        return;
    }
    if (Length <= 0.f || MaxCoins <= 0)
    {
        GRLOG_E("Invalid args: Length=%.2f, MaxCoins=%d", Length, MaxCoins);
        return;
    }

    const FVector Base = GetActorLocation();
    const float StartX = Base.X;
    const float Y = Base.Y;
    const float Z = Base.Z + 100.f;

    const float Spacing = FMath::Max(Length / MaxCoins, 50.f);
    int32 SpawnCnt = 0;

    for (int32 i = 0; i < MaxCoins; ++i)
    {
        const float SpawnX = StartX + i * Spacing;
        const FVector SpawnLoc(SpawnX, Y, Z);
        const FRotator SpawnRot = FRotator::ZeroRotator;

        TrySpawnOne(SpawnLoc, SpawnRot, SpawnCnt);
    }

    GRLOG("Spawn Count : %d", SpawnCnt);
}
