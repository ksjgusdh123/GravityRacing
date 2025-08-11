// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define MAX_ROAD_LINE 4

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GRPlayer.generated.h"


class UInputAction;
struct FInputActionValue;


UCLASS()
class GRAVITYRACING_API AGRPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AGRPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEST")
	bool bTest;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Landed(const FHitResult& Hit) override;

protected:
	UFUNCTION()
	virtual void HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& SweepResult);

public:
	void SetPlayerMaxSpeed(float Speed);
	void SetPlayerBoost();
	void SetRoadOneLineDistance(float dist) { OneLineDistance = dist; }

	void BouncePlayer(const FHitResult& SweepResult);
public:

	UFUNCTION(BlueprintCallable)
	void InGameMove(const FInputActionValue& value);
	UFUNCTION(BlueprintCallable)
	void Flip(const FInputActionValue& value);

	void CrossLine(float DeltaTime);
	void FlipPlayer(float DeltaTime);
	void BoostSpeedTime(float DeltaTime);
	void RecoverCenter(float DeltaTime);
	void ChangeSpeed(float DeltaTime);
	void UpdateSpeed(float DeltaTime);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> IMC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> InGameMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> FlipAction;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BikeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* Camera;


	UPROPERTY()
	USkeletalMeshComponent* PlayerMesh;
	
	UPROPERTY(EditAnywhere)
	float LeanAngle;

private:
	uint32 bIsFlip : 1;
	uint32 bIsCurrentFlipState : 1;
	uint32 bIsMoving : 1;
	uint32 bIsBoosting : 1;
	uint32 bIsRecoverCenter : 1;
	uint32 bIsDie : 1;
	uint32 bIsChangeSpeed : 1;

	float OneLineDistance;
	float OriginalMaxSpeed;
	float TargetMaxSpeed;
	float MaxSpeed;
	float BoostTime;
	float MaxBoostTime;
	float PitchDegree = 0.f;
	float RollDegree = 0.f;
	float ChangeSpeedTime = 0.f;
	float MaxChangeSpeedTime = 10.f;
	float NextSpringArmRot = 20.f;	
		
	FVector TargetLocation;
	FRotator TargetRot;
	FQuat  PrevQuat;
	int32 CurrentLine;
};
