// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GRTunnel.generated.h"

UCLASS()
class GRAVITYRACING_API AGRTunnel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGRTunnel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	float GetTunnelLength() const { return Mesh->Bounds.BoxExtent.X; }

	void SetObstacle(class AGRObstacle* obstacle) { Obstacle = obstacle; }

	void RePositionEvent(TSubclassOf<class AGRObstacle> NewObstacleClass);

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	AGRObstacle* Obstacle;
};
