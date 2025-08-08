// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Obstacle/GRGate.h"
#include "Components/BoxComponent.h"
#include "GravityRacing.h"
#include "Characters/Player/GRPlayer.h"

AGRGate::AGRGate()
	: OpenSpeed(1000.f), TriggerSizeXY(FVector2D(1000.f, 1000.f))
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

	OpenTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	OpenTriggerBox->SetupAttachment(Root);
	OpenTriggerBox->SetBoxExtent(FVector(1000.f, 1000.f, 200.f));
	OpenTriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	OpenTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGRGate::OnOpenTriggerBoxOverlapBegin);
}

void AGRGate::BeginPlay()
{
	Super::BeginPlay();
}

void AGRGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsGateOpen)
	{
		OpenGateEvent(DeltaTime);
	}
}

void AGRGate::SetIsOpenGate(float height)
{
	bool UpFirst = FMath::RandBool();

	OpenTriggerBox->SetBoxExtent(FVector(TriggerSizeXY, height));

	if(UpFirst)
	{
		bIsOpenUpGate = FMath::RandBool();
		if (bIsOpenUpGate)	
		{
			bIsOpenDownGate = FMath::RandBool();
		}
		else
		{
			bIsOpenDownGate = true;
		}
	}
	else
	{
		bIsOpenDownGate = FMath::RandBool();
		if (bIsOpenDownGate)
		{
			bIsOpenUpGate = FMath::RandBool();
		}
		else
		{
			bIsOpenUpGate = true;
		}
	}
}

void AGRGate::HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	GRLOG("Gate Hit Event");
}

void AGRGate::OnOpenTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AGRPlayer>(OtherActor))
	{
		GRLOG("Open Gate");
		bIsGateOpen = true;
	}
}

void AGRGate::OpenGateEvent(float DeltaTime)
{
	if (bIsOpenUpGate) UpGateMesh->AddLocalOffset(FVector(0.f, 0.f, OpenSpeed * DeltaTime));
	if (bIsOpenDownGate) DownGateMesh->AddLocalOffset(FVector(0.f, 0.f, -OpenSpeed * DeltaTime));
}
