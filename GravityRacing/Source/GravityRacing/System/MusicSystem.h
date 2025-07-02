// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MusicSystem.generated.h"


UENUM(BlueprintType)
enum class EGameMusic : uint8
{
	None       UMETA(DisplayName = "None"),
	MainMenu   UMETA(DisplayName = "Main Menu"),
	Run       UMETA(DisplayName = "Run"),
	Jump     UMETA(DisplayName = "Jump"),
};


/**
 * 
 */
UCLASS()
class GRAVITYRACING_API UMusicSystem : public UObject
{
	GENERATED_BODY()
	
	UMusicSystem();
public:
	void Init(UObject* Outer);

	void Play(EGameMusic MusicType, float FadeIn = 1.f);

	void Stop(float FadeOut = 1.f);

	
private:
	UPROPERTY()
	UAudioComponent* AudioComponent;

	UPROPERTY()
	TMap<EGameMusic, USoundBase*> MusicMap;

	UPROPERTY()
	EGameMusic CurrentMusic = EGameMusic::None;

	void RegisterMusic();
};
