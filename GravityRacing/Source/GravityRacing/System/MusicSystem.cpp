// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MusicSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

UMusicSystem::UMusicSystem()
{
	static ConstructorHelpers::FObjectFinder<USoundBase> ThemeMusic(TEXT("/Game/GravityRacing/Audio/BGM/SW_Theme.SW_Theme"));

	if (ThemeMusic.Succeeded())  MusicMap.Add(EGameMusic::MainMenu, ThemeMusic.Object);


}

void UMusicSystem::Init(UObject* Outer)
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(Outer);
	AudioComponent = NewObject<UAudioComponent>(World->GetWorldSettings());
	AudioComponent->RegisterComponentWithWorld(World);
	AudioComponent->bIsUISound = true;
	AudioComponent->bAllowSpatialization = false;
	AudioComponent->SetVolumeMultiplier(1.0f);

	AudioComponent->AttachToComponent(World->GetWorldSettings()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	AudioComponent->Activate(true);

}


void UMusicSystem::Play(EGameMusic MusicType, float FadeIn)
{
	if (CurrentMusic == MusicType) return;

	if (AudioComponent->IsPlaying())
		AudioComponent->FadeOut(0.5f, 0.f);

	if (USoundBase** Found = MusicMap.Find(MusicType))
	{
		AudioComponent->SetSound(*Found);
		AudioComponent->Play();
		CurrentMusic = MusicType;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,                     
				3.0f,                  
				FColor::Green,           
				TEXT("Play: MainMenu BGM")
			);
		}
	}

}

void UMusicSystem::Stop(float FadeOut)
{
	if (AudioComponent && AudioComponent->IsPlaying())
	{
		AudioComponent->FadeOut(FadeOut, 0.f);
		CurrentMusic = EGameMusic::None;
	}
}
