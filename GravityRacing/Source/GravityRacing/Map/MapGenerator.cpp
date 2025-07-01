// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/MapGenerator.h"
#include "Tunnel.h"
#include "../Player/GRPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMapGenerator::AMapGenerator()
	: TunnelLength(2000.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerRef = Cast<AGRPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	for (int32 i = 0; i < MaxTunnelCount; ++i)
	{
		ATunnel* tunnel = GetWorld()->SpawnActor<ATunnel>(
			TunnelClass,
			LastTunnelLocation,
			FRotator::ZeroRotator);

		if (tunnel)
		{
			FString Msg = FString::Printf(TEXT("Spawn!!"));
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Msg);
			ActiveTunnels.Add(tunnel);
			LastTunnelLocation += FVector(TunnelLength, 0.f, 0.f);
		}
	}
}

// Called every frame
void AMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapGenerator::RepositionTunnel()
{
	ATunnel* tunnel = ActiveTunnels[0];
	ActiveTunnels.RemoveAt(0);

	FVector NewLocation = LastTunnelLocation + FVector(TunnelLength, 0, 0);
	tunnel->SetActorLocation(NewLocation);

	LastTunnelLocation = NewLocation;
	ActiveTunnels.Add(tunnel);
}

