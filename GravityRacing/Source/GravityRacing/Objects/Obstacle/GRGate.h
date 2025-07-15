// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Obstacle/GRObstacle.h"
#include "GRGate.generated.h"

/**
 * 
 */
UCLASS()
class GRAVITYRACING_API AGRGate : public AGRObstacle
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGRGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetIsOpenGate();
	void SetIsOpenUpGate(bool b) { bIsOpenUpGate = b; }
	void SetIsOpenDownGate(bool b) { bIsOpenDownGate = b; }

protected:
	virtual void HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void OnOpenTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void OpenGateEvent(float DeltaTime);

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* UpGateMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DownGateMesh;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* OpenTriggerBox;

private:
	int32 bIsGateOpen : 1;
	int32 bIsOpenUpGate : 1;
	int32 bIsOpenDownGate : 1;
		
	float OpenSpeed;

};
