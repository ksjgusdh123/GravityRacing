// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Obstacle/GRMovingObstacle.h"
#include "Characters/Player/GRPlayer.h"

AGRMovingObstacle::AGRMovingObstacle()
	: dir(1)
{
}

void AGRMovingObstacle::Tick(float DeltaSeconds)
{
	FVector right = GetActorRightVector();

	AddActorWorldOffset(right * dir, true);
}

void AGRMovingObstacle::HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	AGRPlayer* player = Cast<AGRPlayer>(OtherActor);
	if (!player)
	{
		FString Msg = FString::Printf(TEXT("withWall!!"), *OtherActor->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Msg);
		dir *= -1;
	}
}
