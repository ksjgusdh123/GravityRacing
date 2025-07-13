// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Obstacle/GRGate.h"
#include "Components/BoxComponent.h"
#include "../../GravityRacing.h"

AGRGate::AGRGate()
{
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	//CollisionBox->SetupAttachment(RootComponent);
	Mesh->SetupAttachment(Root);

	//UpCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("UpCollisionBox"));
	//UpCollisionBox->SetupAttachment(RootComponent);
	/*UpCollisionBox->SetNotifyRigidBodyCollision(true);
	UpCollisionBox->SetCollisionProfileName(TEXT("BlockAll"));
	UpCollisionBox->OnComponentHit.AddDynamic(this, &AGRGate::HitEvent);*/


	UpGateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpGateMesh"));
	UpGateMesh->SetupAttachment(Root);
	//UpGateMesh->SetupAttachment(UpCollisionBox);
	UpGateMesh->SetNotifyRigidBodyCollision(true);
	UpGateMesh->SetCollisionProfileName(TEXT("BlockAll"));
	UpGateMesh->OnComponentHit.AddDynamic(this, &AGRGate::HitEvent);


	/*DownCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DownCollisionBox"));
	DownCollisionBox->SetupAttachment(RootComponent);*/
	//DownCollisionBox->SetNotifyRigidBodyCollision(true);
	//DownCollisionBox->SetCollisionProfileName(TEXT("BlockAll"));
	//DownCollisionBox->OnComponentHit.AddDynamic(this, &AGRGate::HitEvent);

	DownGateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DownGateMesh"));
	DownGateMesh->SetupAttachment(Root);
	//DownGateMesh->SetupAttachment(DownCollisionBox);
	DownGateMesh->SetNotifyRigidBodyCollision(true);
	DownGateMesh->SetCollisionProfileName(TEXT("BlockAll"));
	DownGateMesh->OnComponentHit.AddDynamic(this, &AGRGate::HitEvent);
}

void AGRGate::BeginPlay()
{
	Super::BeginPlay();

	//if (UpGateMesh && UpCollisionBox)
	//{
	//	FVector extent, origin;
	//	UpGateMesh->GetLocalBounds(origin, extent);

	//	UpCollisionBox->SetBoxExtent(extent);
	//	//UpCollisionBox->SetRelativeLocation(FVector(origin));
	//}

	//if (DownGateMesh && DownCollisionBox)
	//{
	//	FVector extent, origin;
	//	DownGateMesh->GetLocalBounds(origin, extent);

	//	DownCollisionBox->SetBoxExtent(extent);
	//	//DownCollisionBox->SetRelativeLocation(FVector(origin));
	//}
}

void AGRGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	AddActorWorldOffset(GetActorForwardVector() * 100 * DeltaTime, true);

}

void AGRGate::HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	GRLOG("Gate Hit Event");
}
