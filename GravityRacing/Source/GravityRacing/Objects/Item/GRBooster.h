// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GRBooster.generated.h"

UCLASS()
class GRAVITYRACING_API AGRBooster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGRBooster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetRoadOneLineDistance(float dist) { OneLineDistance = dist; }
	void SetCurrentLine(int32 line) { CurrentLine = line; }
	virtual void SpawnBooster(int32 line);

protected:
	UFUNCTION()
	virtual void OverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

private:
	float OneLineDistance;
	float TunnelHeight;

	int32 CurrentLine;
};
