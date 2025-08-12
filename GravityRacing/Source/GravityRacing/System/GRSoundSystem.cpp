// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GRSoundSystem.h"
#include "GravityRacing.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

UGRSoundSystem::UGRSoundSystem()
{
    static ConstructorHelpers::FObjectFinder<USoundBase> MainBGMObj(TEXT("/Game/GravityRacing/Audio/BGM/Cyber_Punk_Music_v11/Sound_Cues/MainBGM.MainBGM"));
    if (MainBGMObj.Succeeded())
    {
        MainBGM = MainBGMObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> LobbyBGMObj(TEXT("/Game/GravityRacing/Audio/BGM/Cyber_Punk_Music_v11/Sound_Waves/LobbyBGM.LobbyBGM"));
    if (LobbyBGMObj.Succeeded())
    {
        LobbyBGM = LobbyBGMObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> CoinObj(TEXT("/Game/GravityRacing/Audio/SFX/Coin/SFX_Coin.SFX_Coin"));
    if (CoinObj.Succeeded())
    {
        CoinSFX = CoinObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> BoostObj(TEXT("/Game/GravityRacing/Audio/SFX/Booster/SFX_Boost.SFX_Boost"));
    if (BoostObj.Succeeded())
    {
        BoostSFX = BoostObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> DieObj(TEXT("/Game/GravityRacing/Audio/SFX/Booster/SFX_Die.SFX_Die"));
    if (DieObj.Succeeded())
    {
        DieSFX = DieObj.Object;
    }
}

void UGRSoundSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // 필요하다면 여기서 미리 PreloadAll 호출
    PreloadAll();
}

USoundBase* UGRSoundSystem::GetSoundSync(EGameSound Id)
{
    TSoftObjectPtr<USoundBase> Asset;

    switch (Id)
    {
    case EGameSound::MainBGM:   Asset = MainBGM; break;
    case EGameSound::LobbyBGM:  Asset = LobbyBGM; break;
    case EGameSound::Coin:      Asset = CoinSFX; break;
    case EGameSound::Boost:     Asset = BoostSFX; break;
    case EGameSound::Die:       Asset = DieSFX; break;
    default: return nullptr;
    }

    return Asset.IsValid() ? Asset.Get() : Asset.LoadSynchronous();
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
        if (UWorld* World = GetWorld())
        {
            const float FadeTime = 0.5f;

            if (IsValid(BGMComp))
            {
                BGMComp->FadeOut(FadeTime, 0.f);
                BGMComp = nullptr;
            }

            UAudioComponent* NewComp = UGameplayStatics::SpawnSound2D(World, S, 1.f, 1.f, 0.f, nullptr, false);
            if (IsValid(NewComp))
            {
                NewComp->bAutoDestroy = false;
                NewComp->bStopWhenOwnerDestroyed = false;
                NewComp->OnAudioFinishedNative.RemoveAll(this);
                NewComp->OnAudioFinishedNative.AddUObject(this, &UGRSoundSystem::HandleBGMFinished);
                NewComp->FadeIn(FadeTime, 1.f);
                BGMComp = NewComp;
            }
        }
    }
}

void UGRSoundSystem::StopBGM()
{
    if (BGMComp && BGMComp->IsPlaying())
    {
        BGMComp->FadeOut(0.5f, 0.f);
    }
}

void UGRSoundSystem::HandleBGMFinished(UAudioComponent* AC)
{
    if (IsValid(BGMComp))
    {
        BGMComp->Play(0.f);
    }
}

void UGRSoundSystem::Preload(EGameSound Id)
{
    if (UAssetManager* AM = UAssetManager::GetIfValid())
    {
        if (USoundBase* S = GetSoundSync(Id))
        {
        }
    }
}

void UGRSoundSystem::PreloadAll()
{
    for (uint8 i = 0; i <= (uint8)EGameSound::Die; ++i)
    {
        Preload((EGameSound)i);
    }
}