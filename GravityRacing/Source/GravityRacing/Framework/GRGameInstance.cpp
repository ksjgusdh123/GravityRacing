// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/GRGameInstance.h"


void UGRGameInstance::Init()
{
	Super::Init();

	MusicSystem = NewObject<UMusicSystem>(this);
	MusicSystem->Init(this);


	MusicSystem->Play(EGameMusic::MainMenu);
	
}

void UGRGameInstance::Shutdown()
{
	Super::Shutdown();
}

