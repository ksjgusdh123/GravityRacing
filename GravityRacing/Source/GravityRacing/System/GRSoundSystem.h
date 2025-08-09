// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundClass.h"
#include "GRSoundSystem.generated.h"

UENUM(BlueprintType)
enum class EGameSound : uint8
{
    None       UMETA(DisplayName = "None"),
    MainBGM   UMETA(DisplayName = "Main BGM"),
    LobbyBGM   UMETA(DisplayName = "Looby BGM"),
	Coin	   UMETA(DisplayName = "Coin"),
	Boost	   UMETA(DisplayName = "Boost"),
	Die	   UMETA(DisplayName = "Die"),
};

USTRUCT(BlueprintType)
struct FSoundData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGameSound SoundId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<USoundBase> SoundAsset;
};

UCLASS()
class GRAVITYRACING_API UGRSoundSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    void SetSoundTable(UDataTable* InTable);
    void BuildMap();

    void Play2D(EGameSound Id);
    void PlayBGM(EGameSound Id);
    void StopBGM();
    void HandleBGMFinished(UAudioComponent* AC);

    void Preload(EGameSound Id);
    void PreloadAll();
private:
    UPROPERTY()
    UDataTable* SoundTable = nullptr;
    UPROPERTY()
    TMap<EGameSound, TSoftObjectPtr<USoundBase>> SoundMap;
    UPROPERTY(Transient)
    UAudioComponent* BGMComp = nullptr;

    UFUNCTION()
    USoundBase* GetSoundSync(EGameSound Id);
};
