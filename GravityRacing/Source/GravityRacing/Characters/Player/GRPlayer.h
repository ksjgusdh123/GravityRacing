// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define MAX_ROAD_LINE 4

#include "System/MusicSystem.h"
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
	// Sets default values for this character's properties
	AGRPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit) override;


public:
	void SetPlayerMaxSpeed(float Speed);

public:
	void Move(const FInputActionValue& value);
	void InGameMove(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void JumpStart(const FInputActionValue& value);
	void JumpStop(const FInputActionValue& value);
	void Flip(const FInputActionValue& value);

	void CrossLine(float DeltaTime);
	void FlipPlayer(float DeltaTime);
	void BoostSpeedTime(float DeltaTime);
	
public:
	void PlayMusic(EGameSound SoundType);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound)
	class USoundBase* Coin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound)
	class USoundBase* Boost;




private:
	uint32 bIsFlip : 1;
	uint32 bIsCurrentFlipState : 1;
	uint32 bIsMoving : 1;
	uint32 bIsBoosting : 1;

	float RoadDistance;
	float OriginalMaxSpeed;
	float BoostTime;
	float MaxBoostTime;
	float PitchDegree = 0.f;
	float RollDegree = 0.f;

	FVector TargetLocation;
	FQuat  PrevQuat;
	int32 CurrentLine;
};
