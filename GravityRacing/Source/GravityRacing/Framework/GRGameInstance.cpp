// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/GRGameInstance.h"
#include "System/GPSoundSystem.h"

void UGRGameInstance::Init()
{
	Super::Init();
}

void UGRGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UGRGameInstance::OnStart()
{
	Super::OnStart();
	auto* SoundSys = GetSubsystem<UGPSoundSystem>();
	SoundSys->PlayBGM(EGameSound::MainBGM);
}

