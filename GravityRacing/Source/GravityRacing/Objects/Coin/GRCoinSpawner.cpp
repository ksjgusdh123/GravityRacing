#include "Objects/Coin/GRCoinSpawner.h"
#include "GravityRacing.h"
#include "Objects/Coin/GRCoin.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Map/GRMapGenerator.h"

AGRCoinSpawner::AGRCoinSpawner()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGRCoinSpawner::BeginPlay()
{
    Super::BeginPlay();
    TunnelLength = AGRMapGenerator::TunnelLength;
    OneLineDistance = TunnelLength.Y / 4;

    AGRMapGenerator* Generator = Cast<AGRMapGenerator>(UGameplayStatics::GetActorOfClass(GetWorld(), AGRMapGenerator::StaticClass()));
    if (Generator)
    {
        Generator->OnTunnelRepositioned.AddDynamic(this, &AGRCoinSpawner::SpawnCoin);
    }
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

bool AGRCoinSpawner::TrySpawnOne(const FVector& Loc, const FRotator& Rot)
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

    return true;
}

void AGRCoinSpawner::DecideTargetCoinLocation()
{
    NowSpawnLine = FMath::RandRange(1, 4);
}

void AGRCoinSpawner::SpawnCoin(FVector TunnelLocation)
{
    if (!CoinClass)
    {
        GRLOG_E("CoinClass is null");
        return;
    }

    DecideTargetCoinLocation();
    const float StartX = TunnelLocation.X;
    const float Y = -OneLineDistance * 1.5 + OneLineDistance * (NowSpawnLine - 1);
    const float Z = 100.f;

    const float Spacing = 100.f;

    float SpawnCount = TunnelLength.X / Spacing - 1;
    for (int i = 0; i < SpawnCount; ++i)
    {
        const float SpawnX = StartX + i * Spacing;
        const FVector SpawnLoc(SpawnX, Y, Z);
        const FRotator SpawnRot = FRotator::ZeroRotator;
        TrySpawnOne(SpawnLoc, SpawnRot);
    }
}

void AGRCoinSpawner::SpawnCoinsInLength(float Length, int32 MaxCoins)
{
 /*   if (!CoinClass)
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

    GRLOG("Spawn Count : %d", SpawnCnt);*/
}
