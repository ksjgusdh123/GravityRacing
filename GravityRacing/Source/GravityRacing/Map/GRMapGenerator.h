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

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGRTunnel> TunnelClass;

	UPROPERTY(EditAnywhere)
	int32 MaxTunnelCount = 5;

	UPROPERTY(EditAnywhere)
	float TunnelLength;

	class AGRPlayer* PlayerRef;

	TArray<AGRTunnel*> ActiveTunnels;
	FVector NextSpawnLocation;
	FVector LastTunnelLocation;


};
