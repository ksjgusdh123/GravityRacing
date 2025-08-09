// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GRGameMode.h"
#include "Map/GRTunnel.h"
#include "Map/GRMapGenerator.h"
#include "Characters/Player/GRPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "System/GPSoundSystem.h"
#include "System/GPUISystem.h"

void AGRGameMode::BeginPlay()
{
	Super::BeginPlay();

	AGRPlayer* Player = Cast<AGRPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		Player->SetRoadOneLineDistance(AGRMapGenerator::OneLineLengthY);
	}
	UGPSoundSystem* SoundSys = GetGameInstance()->GetSubsystem<UGPSoundSystem>();
	if(SoundSys)
	{
		SoundSys->PlayBGM(EGameSound::MainBGM);
	}
	UGPUISystem* UISys = GetGameInstance()->GetSubsystem<UGPUISystem>();
	if (UISys)
	{
		UISys->OnGameStart();
	}
}
