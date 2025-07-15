// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GRMapGenerator.generated.h"

UCLASS()
class GRAVITYRACING_API AGRMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGRMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RepositionTunnel();

public:
	static float OneLineLengthY;

	// X -> 터널의 X축 총 길이, Y -> 터널의 4차선의 길이
	static FVector2D TunnelLength;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGRTunnel> TunnelClass;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AGRObstacle>> ObstacleClasses;

	UPROPERTY(EditAnywhere)
	int32 MaxTunnelCount = 5;


	UPROPERTY()
	class AGRPlayer* PlayerRef;

	UPROPERTY()
	TArray<AGRTunnel*> ActiveTunnels;

	int32 ObstaclesCount;
	FVector NextSpawnLocation;
	FVector LastTunnelLocation;
};
