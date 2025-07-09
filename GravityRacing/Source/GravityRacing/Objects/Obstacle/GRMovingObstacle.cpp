// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Obstacle/GRMovingObstacle.h"
#include "Characters/Player/GRPlayer.h"
#include "../../GravityRacing.h"

AGRMovingObstacle::AGRMovingObstacle()
	: Dir(FVector(1.f, 0.f, 0.f)), Speed(FMath::RandRange(3, 20)), PushDistance(5.f)
{
}

void AGRMovingObstacle::Tick(float DeltaSeconds)
{
	FVector right = GetActorRightVector();

	AddActorWorldOffset(Dir * Speed, true);
}

void AGRMovingObstacle::HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	AGRPlayer* player = Cast<AGRPlayer>(OtherActor);
	if (!player)
	{
		GRLOG("With Wall");
		Dir *= -1;

		FVector PushOffset = SweepResult.ImpactNormal * PushDistance;
		SetActorLocation(GetActorLocation() + PushOffset);
	}
}
