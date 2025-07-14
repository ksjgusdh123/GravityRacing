// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Obstacle/GRObstacle.h"
#include "Components/BoxComponent.h"
#include "../../GravityRacing.h"

// Sets default values
AGRObstacle::AGRObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetMobility(EComponentMobility::Movable);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetCollisionProfileName(TEXT("BlockAll"));
	Mesh->OnComponentHit.AddDynamic(this, &AGRObstacle::HitEvent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/EditorMeshes/EditorCube.EditorCube"));

	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void AGRObstacle::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGRObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGRObstacle::HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	GRLOG("NOTIFYOVERRIDE");
}

