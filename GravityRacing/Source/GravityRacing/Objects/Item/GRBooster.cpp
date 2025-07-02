// Fill out your copyright notice in the Description page of Project Settings.


#include "GRBooster.h"
#include "GravityRacing.h"
#include "Components/BoxComponent.h"
#include "../../Characters/Player/GRPlayer.h"

// Sets default values
AGRBooster::AGRBooster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGRBooster::OverlapEvent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CollisionBox);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/EditorMeshes/EditorCube.EditorCube"));

	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void AGRBooster::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGRBooster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGRBooster::OverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGRPlayer* player = Cast<AGRPlayer>(OtherActor);
	if (player)
	{
		player->SetPlayerMaxSpeed(1800.f);
		GRLOG("Boost!!");
	}
}

