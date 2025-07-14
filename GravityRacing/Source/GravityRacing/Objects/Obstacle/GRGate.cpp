// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Obstacle/GRGate.h"
#include "Components/BoxComponent.h"
#include "../../GravityRacing.h"

AGRGate::AGRGate()
{
	Mesh->SetupAttachment(Root);

	UpGateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpGateMesh"));
	UpGateMesh->SetupAttachment(Root);
	UpGateMesh->SetNotifyRigidBodyCollision(true);
	UpGateMesh->SetCollisionProfileName(TEXT("BlockAll"));
	UpGateMesh->OnComponentHit.AddDynamic(this, &AGRGate::HitEvent);

	DownGateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DownGateMesh"));
	DownGateMesh->SetupAttachment(Root);
	DownGateMesh->SetNotifyRigidBodyCollision(true);
	DownGateMesh->SetCollisionProfileName(TEXT("BlockAll"));
	DownGateMesh->OnComponentHit.AddDynamic(this, &AGRGate::HitEvent);
}

void AGRGate::BeginPlay()
{
	Super::BeginPlay();
}

void AGRGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGRGate::HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	GRLOG("Gate Hit Event");
}
