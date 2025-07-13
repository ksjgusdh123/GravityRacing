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
	inline void SetObstacleDir(const FVector& dir) { Dir = dir; }

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds);

protected:
	virtual void HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& SweepResult) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	float Speed;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	FVector Dir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	float PushDistance;

protected:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

private:


};
