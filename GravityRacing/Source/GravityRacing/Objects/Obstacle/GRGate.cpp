// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Obstacle/GRGate.h"
#include "Components/BoxComponent.h"
#include "../../GravityRacing.h"

AGRGate::AGRGate()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CollisionBox->SetupAttachment(RootComponent);
	Mesh->SetupAttachment(CollisionBox);

	UpCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("UpCollisionBox"));
	UpGateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpGateMesh"));
	UpCollisionBox->SetupAttachment(RootComponent);
	UpGateMesh->SetupAttachment(UpCollisionBox);

	UpCollisionBox->SetNotifyRigidBodyCollision(true);
	UpCollisionBox->SetCollisionProfileName(TEXT("BlockAll"));
	UpCollisionBox->OnComponentHit.AddDynamic(this, &AGRGate::HitEvent);


	DownCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DownCollisionBox"));
	DownGateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DownGateMesh"));
	DownCollisionBox->SetupAttachment(RootComponent);
	DownGateMesh->SetupAttachment(DownCollisionBox);

	DownCollisionBox->SetNotifyRigidBodyCollision(true);
	DownCollisionBox->SetCollisionProfileName(TEXT("BlockAll"));
	DownCollisionBox->OnComponentHit.AddDynamic(this, &AGRGate::HitEvent);
}

void AGRGate::BeginPlay()
{
	Super::BeginPlay();
}

void AGRGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (UpGateMesh && UpCollisionBox)
	{
		FVector extent, origin;
		UpGateMesh->GetLocalBounds(origin, extent);

		UpCollisionBox->SetBoxExtent(extent);
		UpCollisionBox->SetRelativeLocation(FVector(origin));
	}

	if (DownGateMesh && DownCollisionBox)
	{
		FVector extent, origin;
		DownGateMesh->GetLocalBounds(origin, extent);

		DownCollisionBox->SetBoxExtent(extent);	
		DownCollisionBox->SetRelativeLocation(FVector(origin));
	}*/
}

void AGRGate::HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	GRLOG("Gate Hit Event");
}
