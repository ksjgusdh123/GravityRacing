// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle/RLMovingObstacle.h"
#include "..\Player\GRPlayer.h"

ARLMovingObstacle::ARLMovingObstacle()
	: dir(1)
{
}

void ARLMovingObstacle::Tick(float DeltaSeconds)
{
	FVector right = GetActorRightVector();

	AddActorWorldOffset(right * dir, true);
}

void ARLMovingObstacle::HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	AGRPlayer* player = Cast<AGRPlayer>(OtherActor);
	if (!player)
	{
		FString Msg = FString::Printf(TEXT("withWall!!"), *OtherActor->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Msg);
		dir *= -1;
	}
}
