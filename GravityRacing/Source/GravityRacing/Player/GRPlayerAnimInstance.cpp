// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GRPlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UGRPlayerAnimInstance::UGRPlayerAnimInstance()
{
}

void UGRPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UGRPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		Speed = Velocity.Size2D();
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > 2.f);
	}
}
