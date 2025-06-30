// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle/Obstacle.h"
#include "RLMovingObstacle.generated.h"

/**
 * 
 */
UCLASS()
class GRAVITYRACING_API ARLMovingObstacle : public AObstacle
{
	GENERATED_BODY()

public:
	ARLMovingObstacle();

public:
	virtual void Tick(float DeltaSeconds);

protected:
	virtual void HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& SweepResult) override;

private:
	int8 dir;
};
