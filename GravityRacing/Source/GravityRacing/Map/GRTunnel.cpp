// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/GRTunnel.h"
#include "../Objects/Obstacle/GRObstacle.h"
#include "../GravityRacing.h"

// Sets default values
AGRTunnel::AGRTunnel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	RootComponent->SetMobility(EComponentMobility::Movable);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/GravityRacing/Objects/Obstacle/Assets/Mesh/SM_Tunnel.SM_Tunnel"));

	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void AGRTunnel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGRTunnel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGRTunnel::RePositionEvent(TSubclassOf<class AGRObstacle> NewObstacleClass)
{
	if (Obstacle)
	{
		Obstacle->Destroy();
		Obstacle = nullptr;
	}

	if (NewObstacleClass)
	{
		AGRObstacle* NewObstacle = GetWorld()->SpawnActor<AGRObstacle>(NewObstacleClass, GetActorLocation(), GetActorRotation());
		if (NewObstacle)
		{
			UStaticMeshComponent* ObstacleMesh = NewObstacle->FindComponentByClass<UStaticMeshComponent>();
			if (ObstacleMesh)
			{
				FVector Extent = ObstacleMesh->Bounds.BoxExtent  ;

				FVector OriginLocation = GetActorLocation();
				OriginLocation.Z += Extent.Z;
				NewObstacle->SetActorLocation(OriginLocation);

				GRLOG("Obstacle Extent: %s", *Extent.ToString());
			}

			Obstacle = NewObstacle;
		}
	}
}

