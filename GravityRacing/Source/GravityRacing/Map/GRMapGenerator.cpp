// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/GRMapGenerator.h"
#include "GRTunnel.h"
#include "../Characters/Player/GRPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "../Objects/Obstacle/GRObstacle.h"
#include "Objects/Obstacle/GRGate.h"

float AGRMapGenerator::OneLineLengthY = 0.f;
FVector2D AGRMapGenerator::TunnelLength = FVector2D(0.f, 0.f);

// Sets default values
AGRMapGenerator::AGRMapGenerator()
	: ObstaclesCount(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGRMapGenerator::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<AGRPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	for (int32 i = 0; i < MaxTunnelCount; ++i)
	{
		AGRTunnel* tunnel = GetWorld()->SpawnActor<AGRTunnel>(
			TunnelClass,
			LastTunnelLocation,
			FRotator(0.f, 0.f, 0.f));


		if (tunnel)
		{
			TunnelLength.X = tunnel->GetTunnelLengthX();
			ActiveTunnels.Add(tunnel);
			LastTunnelLocation += FVector(TunnelLength.X, 0.f, 0.f);
		}
	}
	LastTunnelLocation -= FVector(TunnelLength.X, 0.f, 0.f);

	OneLineLengthY = ActiveTunnels[0]->GetTunnelOneLineLengthY();
	TunnelLength.Y = ActiveTunnels[0]->GetTunnelLinesLengthY();

	ObstaclesCount = ObstacleClasses.Num();
}

// Called every frame
void AGRMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!PlayerRef) return;

	FVector PlayerLocation = PlayerRef->GetActorLocation();
	float DistFinalTunnel = FVector::Dist(PlayerLocation, ActiveTunnels[0]->GetActorLocation());

	if (DistFinalTunnel > TunnelLength.X * (MaxTunnelCount / 2))
	{
		RepositionTunnel();
	}
}

void AGRMapGenerator::RepositionTunnel()
{
	AGRTunnel* tunnel = ActiveTunnels[0];
	ActiveTunnels.RemoveAt(0);
	tunnel->DestroyObstacles();
	tunnel->DestroyCoins();

	FVector NewLocation = LastTunnelLocation + FVector(TunnelLength.X, 0, 0);
	tunnel->SetActorLocation(NewLocation);

	int32 ObstacleType = FMath::RandRange(0, ObstaclesCount * 2);

	if (ObstacleType == 0)
	{
		// 터널인 경우
		tunnel->RePositionGate();
	}
	if (ObstacleType == ObstaclesCount * 2)
	{
		tunnel->RePositionBooster();
	}
	else
	{
		// 터널이 아닌 경우
		for (int i = 0; i < 3; ++i)
		{
			TSubclassOf<AGRObstacle> ObstacleClass = ObstacleClasses[FMath::RandRange(0, ObstaclesCount - 1)];

			tunnel->RePositionEvent(ObstacleClass, i);
		}
	}
	LastTunnelLocation = NewLocation;
	ActiveTunnels.Add(tunnel);
	OnTunnelRepositioned.Broadcast(NewLocation, tunnel);
}