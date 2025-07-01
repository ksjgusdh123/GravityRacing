// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Tunnel.h"

// Sets default values
ATunnel::ATunnel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Assets/Tunnel/mesh/tunnel.tunnel"));

	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void ATunnel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATunnel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

