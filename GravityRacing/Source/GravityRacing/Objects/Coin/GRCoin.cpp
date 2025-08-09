// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Coin/GRCoin.h"
#include "GravityRacing.h"  
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/Player/GRPlayer.h" 
#include "Framework/GRGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "System/GPSoundSystem.h"

AGRCoin::AGRCoin()
{
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->InitSphereRadius(50.f);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionObjectType(ECC_WorldDynamic);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Collision->SetGenerateOverlapEvents(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGRCoin::BeginPlay()
{
	Super::BeginPlay();
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AGRCoin::OnOverlapBegin);
}

void AGRCoin::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bPickedUp) return;

    if (AGRPlayer* Player = Cast<AGRPlayer>(OtherActor))
    {
		auto* SoundSys = GetGameInstance()->GetSubsystem<UGPSoundSystem>();
		SoundSys->Play2D(EGameSound::Coin);

        bPickedUp = true;

        Collision->SetGenerateOverlapEvents(false);
        Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


        if (auto* GameMode = Cast<AGRGameModeBase>(UGameplayStatics::GetGameMode(this)))
        {
            GameMode->AddScore(Value);
        }

        Destroy();
    }
}
