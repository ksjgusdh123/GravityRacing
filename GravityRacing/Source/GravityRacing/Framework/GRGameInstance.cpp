// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/GRGameInstance.h"


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
	if (MusicSystem == nullptr)
	{
		MusicSystem = NewObject<UMusicSystem>(this);
		MusicSystem->Init(this);
	}

	MusicSystem->Play(EGameMusic::MainMenu);

}

