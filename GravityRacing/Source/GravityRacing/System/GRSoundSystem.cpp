// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GRSoundSystem.h"
#include "GravityRacing.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

void UGRSoundSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (!SoundTable)
    {
        static const TCHAR* Path = TEXT("/Game/GravityRacing/Data/DT_SoundTable");
        SoundTable = LoadObject<UDataTable>(nullptr, Path);
        if (!SoundTable)
        {
            GRLOG("[SoundManager] Failed to load DataTable: %s", Path);
            return;
        }
    }

    BuildMap();
}

void UGRSoundSystem::SetSoundTable(UDataTable* InTable)
{
    SoundTable = InTable;
    BuildMap();
}

void UGRSoundSystem::BuildMap()
{
    SoundMap.Reset();
    if (!SoundTable) return;

    TArray<FSoundData*> Rows;
    SoundTable->GetAllRows<FSoundData>(TEXT("SoundReload"), Rows);
    for (const FSoundData* Row : Rows)
    {
        if (Row && Row->SoundId != EGameSound::None)
            SoundMap.Add(Row->SoundId, Row->SoundAsset);
    }
}


USoundBase* UGRSoundSystem::GetSoundSync(EGameSound Id)
{
    if (Id == EGameSound::None) return nullptr;
    if (TSoftObjectPtr<USoundBase>* Found = SoundMap.Find(Id))
    {
        return Found->IsValid() ? Found->Get() : Found->LoadSynchronous();
    }
    return nullptr;
}

void UGRSoundSystem::Play2D(EGameSound Id)
{
    if (USoundBase* S = GetSoundSync(Id))
    {
        if (UWorld* World = GetWorld())
        {
            UGameplayStatics::PlaySound2D(World, S);
        }
    }
}

void UGRSoundSystem::PlayBGM(EGameSound Id)
{
    if (USoundBase* S = GetSoundSync(Id))
    {
        UWorld* World = GetWorld();
        if (!World) return;

        const float FadeTime = 0.5f;

        if (IsValid(BGMComp))
        {
            UAudioComponent* Prev = BGMComp;
            BGMComp = nullptr;
            Prev->FadeOut(FadeTime, 0.f);
        }

        BGMComp = UGameplayStatics::CreateSound2D(
            World, S, /*Volume*/1.f, /*Pitch*/1.f, /*StartTime*/0.f, /*Concurrency*/nullptr,
            /*PersistAcrossLevelTransition*/ true
        );
        if (IsValid(BGMComp))
        {
            BGMComp->bAutoDestroy = false;
            BGMComp->FadeIn(FadeTime, 1.f);
        }
    }
}


void UGRSoundSystem::StopBGM()
{
    if (BGMComp && BGMComp->IsPlaying())
    {
        float FadeTime = 0.5f;
        BGMComp->FadeOut(FadeTime, 0.f);
    }
}

void UGRSoundSystem::Preload(EGameSound Id)
{
    if (TSoftObjectPtr<USoundBase>* Found = SoundMap.Find(Id))
    {
        if (!Found->IsValid())
        {
            if (UAssetManager* AM = UAssetManager::GetIfValid())
            {
                AM->GetStreamableManager().RequestSyncLoad(Found->ToSoftObjectPath());
            }
            else
            {
                Found->LoadSynchronous();
            }
        }
    }
}

void UGRSoundSystem::PreloadAll()
{
    if (!SoundTable) return;

    TArray<FSoundData*> Rows;
    SoundTable->GetAllRows<FSoundData>(TEXT("SoundPreloadAll"), Rows);

    if (UAssetManager* AM = UAssetManager::GetIfValid())
    {
        TArray<FSoftObjectPath> Paths;
        for (const FSoundData* Row : Rows)
        {
            if (Row)
            {
                const auto& P = Row->SoundAsset.ToSoftObjectPath();
                if (P.IsValid()) Paths.Add(P);
            }
        }
        if (Paths.Num() > 0)
            AM->GetStreamableManager().RequestSyncLoad(Paths);
    }
    else
    {
        for (const FSoundData* Row : Rows)
            if (Row) Row->SoundAsset.LoadSynchronous();
    }
}