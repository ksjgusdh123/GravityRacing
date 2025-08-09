// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GRLobbyMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "System/GRSoundSystem.h"
#include "System/GRUISystem.h"

void AGRLobbyMode::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

	TArray<AActor*> Cams;
	UGameplayStatics::GetAllActorsWithTag(this, FName("StartCam"), Cams);
	if (AActor* Cam = (Cams.Num() > 0 ? Cams[0] : nullptr))
	{
		PC->SetViewTargetWithBlend(Cam, /*BlendTime=*/0.0f);
	}

	UGRSoundSystem* SoundSys = GetGameInstance()->GetSubsystem<UGRSoundSystem>();
	if (SoundSys)
	{
		SoundSys->PlayBGM(EGameSound::LobbyBGM);
	}
	UGRUISystem* UISys = GetGameInstance()->GetSubsystem<UGRUISystem>();
	if (UISys)
	{
		UISys->OnLobby();
	}
}
