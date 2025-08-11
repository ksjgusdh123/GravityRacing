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
	float GetTunnelLengthX() const { return Mesh->Bounds.BoxExtent.X * 2; }
	float GetTunnelHeight() const { return Mesh->Bounds.BoxExtent.Z * 2; }
	float GetTunnelLinesLengthY() const;
	float GetTunnelOneLineLengthY() const;

	//void SetObstacle(class AGRObstacle* obstacle) { Obstacle = obstacle; }

	void GetCoin(class AGRCoin* Coin, int32 idx);

	void DestroyObstacles();
	void DestroyCoins();
	void RePositionEvent(TSubclassOf<class AGRObstacle> NewObstacleClass, int idx);
	void RePositionGate();
	void RePositionBooster();

public:
	UPROPERTY(EditAnywhere, Category = Gate)
	TSubclassOf<AGRObstacle> GateClass;

	UPROPERTY(EditAnywhere, Category = Booster)
	TSubclassOf<class AGRBooster> BoosterClass;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	TArray<AGRObstacle*> Obstacles;

	UPROPERTY()
	AGRBooster* Booster;

	UPROPERTY()
	TArray<class AGRCoin*> Coins;	
	
	UPROPERTY(EditAnywhere)
	class UArrowComponent* StartRoadArrow;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* EndRoadArrow;

private:
	TArray<float> ObstacleArrayLine;
};
	