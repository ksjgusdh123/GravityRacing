// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GRObstacle.generated.h"

UCLASS()
class GRAVITYRACING_API AGRObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGRObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SpawnObstacle(int32 line);

public:
	void SetRoadOneLineDistance(float dist) { OneLineDistance = dist; }
	void SetCurrentLine(int32 line) { CurrentLine = line; }

protected:
	UFUNCTION()
	virtual void HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

private:
	float OneLineDistance;
	float TunnelHeight;

	int32 CurrentLine;
};
