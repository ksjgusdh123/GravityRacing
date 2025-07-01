// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Obstacle/GRObstacle.h"
#include "GRMovingObstacle.generated.h"

/**
 * 
 */
UCLASS()
class GRAVITYRACING_API AGRMovingObstacle : public AGRObstacle
{
	GENERATED_BODY()

public:
	AGRMovingObstacle();

public:
	virtual void Tick(float DeltaSeconds);

protected:
	virtual void HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& SweepResult) override;

private:
	int8 dir;
};
