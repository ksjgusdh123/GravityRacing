// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Obstacle/GRMovingObstacle.h"
#include "Characters/Player/GRPlayer.h"
#include "Components/BoxComponent.h"
#include "../../GravityRacing.h"

AGRMovingObstacle::AGRMovingObstacle()
	: Dir(FVector(1.f, 0.f, 0.f)), Speed(FMath::RandRange(300.f, 2000.f)), PushDistance(5.f)
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	//CollisionBox->SetupAttachment(RootComponent);
	SetRootComponent(CollisionBox);
	CollisionBox->SetNotifyRigidBodyCollision(true);
	CollisionBox->SetCollisionProfileName(TEXT("BlockAll"));
	CollisionBox->OnComponentHit.AddDynamic(this, &AGRMovingObstacle::HitEvent);

	Mesh->SetupAttachment(CollisionBox);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->SetNotifyRigidBodyCollision(false);
}

void AGRMovingObstacle::BeginPlay()
{
	Super::BeginPlay();

	if (Mesh && CollisionBox)
	{
		FVector extent, origin;
		Mesh->GetLocalBounds(origin, extent);

		CollisionBox->SetBoxExtent(extent);
		//CollisionBox->SetRelativeLocation(FVector(origin));
		//FVector WorldLocation = GetActorLocation();
		//FVector originOffset = FVector(WorldLocation.X, WorldLocation.Y, WorldLocation.Z + extent.Z * 0.5f);
		//SetActorRelativeLocation(FVector(originOffset));
		//Mesh->SetRelativeLocation(FVector(origin));
	}
}

void AGRMovingObstacle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FVector right = GetActorRightVector();

	AddActorWorldOffset(Dir * Speed * DeltaSeconds, true);
}

void AGRMovingObstacle::HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	AGRPlayer* player = Cast<AGRPlayer>(OtherActor);
	GRLOG("With Wall");
	if (!player)
	{
		GRLOG("With Wall");
		Dir *= -1;

		FVector PushOffset = SweepResult.ImpactNormal * PushDistance;
		SetActorLocation(GetActorLocation() + PushOffset);
	}
}
