// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/GRTunnel.h"
#include "Objects/Obstacle/GRObstacle.h"
#include "Objects/Obstacle/GRGate.h"
#include "GravityRacing.h"
#include "Components/ArrowComponent.h"

// Sets default values
AGRTunnel::AGRTunnel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	RootComponent->SetMobility(EComponentMobility::Movable);

	StartRoadArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("StartArrow"));
	StartRoadArrow->SetupAttachment(Mesh);

	EndRoadArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("EndArrow"));
	EndRoadArrow->SetupAttachment(Mesh);


	Mesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	StartRoadArrow->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	StartRoadArrow->SetRelativeLocation(FVector(-740.f, 0.f, 0.f));

	EndRoadArrow->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	EndRoadArrow->SetRelativeLocation(FVector(740.f, 0.f, 0.f));

}

// Called when the game starts or when spawned
void AGRTunnel::BeginPlay()
{
	Super::BeginPlay();
	Obstacles.SetNum(3);

	ObstacleArrayLine.Push(-(GetTunnelLengthX() / 4));
	ObstacleArrayLine.Push(0);
	ObstacleArrayLine.Push((GetTunnelLengthX() / 4));
}

// Called every frame
void AGRTunnel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetTunnelOneLineLengthY();
}

float AGRTunnel::GetTunnelLinesLengthY() const
{
	float len = EndRoadArrow->GetComponentLocation().Y - StartRoadArrow->GetComponentLocation().Y;
	return len;
}

float AGRTunnel::GetTunnelOneLineLengthY() const
{
	return GetTunnelLinesLengthY() / 4;
}

void AGRTunnel::DestroyObstacles()
{
	for (int i = 0; i < 3; ++i)
	{
		if (!Obstacles[i]) continue;

		Obstacles[i]->Destroy();
		Obstacles[i] = nullptr;
	}
}

void AGRTunnel::RePositionEvent(TSubclassOf<AGRObstacle> NewObstacleClass, int idx)
{
	if (Obstacles[idx])
	{
		Obstacles[idx]->Destroy();
		Obstacles[idx] = nullptr;
	}

	if (NewObstacleClass)
	{
		FVector Location = GetActorLocation();

		Location.X += ObstacleArrayLine[idx];
		AGRObstacle* NewObstacle = GetWorld()->SpawnActor<AGRObstacle>(NewObstacleClass, Location, GetActorRotation());
		if (NewObstacle)
		{
			NewObstacle->SetRoadOneLineDistance(GetTunnelOneLineLengthY());

			FVector OriginLocation = NewObstacle->GetActorLocation();
			bool IsFlip = FMath::RandBool();

			FRotator Rot = GetActorRotation();
			if (IsFlip)
			{
				Rot.Yaw = 270.f;
				Rot.Roll = 180.f;
				
				OriginLocation.Z = 700.f;
				NewObstacle->SetActorLocation(OriginLocation);
			}
			else
			{
				Rot.Yaw = 90.f;
				Rot.Roll = 0.f;
			}

			NewObstacle->SetActorRotation(Rot);
			NewObstacle->SpawnObstacle(FMath::RandRange(1, 4));
			Obstacles[idx] = NewObstacle;
		}
	}
}

void AGRTunnel::RePositionGate()
{
	if (Obstacles[0])
	{
		Obstacles[0]->Destroy();
		Obstacles[0] = nullptr;
	}

	FRotator Rot = GetActorRotation();
	Rot.Yaw = (FMath::RandBool() ? 90.f : 270.f);

	AGRObstacle* NewObstacle = GetWorld()->SpawnActor<AGRObstacle>(GateClass, GetActorLocation(), Rot);
	if (NewObstacle)
	{
		NewObstacle->SetRoadOneLineDistance(GetTunnelOneLineLengthY());

		if (AGRGate* Gate = Cast<AGRGate>(NewObstacle))
		{
			Gate->SetIsOpenGate(GetTunnelHeight());
		}
		Obstacles[0] = NewObstacle;
	}
}

