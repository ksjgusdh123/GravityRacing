// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Coin/GRCoin.h"
#include "GravityRacing.h"  
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/Player/GRPlayer.h" 
#include "Framework/GRGameModeBase.h"
#include "Kismet/GameplayStatics.h"

AGRCoin::AGRCoin()
{
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;
	Collision->SetSphereRadius(50.f);
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

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
	if (AGRPlayer* Player = Cast<AGRPlayer>(OtherActor))
	{
		Player->PlayMusic(EGameSound::Coin);
		GRLOG("Coin!");
		auto* GameMode = Cast<AGRGameModeBase>(UGameplayStatics::GetGameMode(this));
		if (GameMode)
		{
			GameMode->AddScore(1);
		}
		Destroy();
	}
}
