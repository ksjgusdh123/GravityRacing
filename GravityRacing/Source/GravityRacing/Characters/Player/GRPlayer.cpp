// Fill out your copyright notice in the Description page of Project Settings.


#include "GRPlayer.h"
#include "GravityRacing.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Objects/Obstacle/GRObstacle.h"
#include "Components/CapsuleComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Sound/SoundCue.h>


// Sets default values
AGRPlayer::AGRPlayer()
	: OneLineDistance(0.f), CurrentLine(1), bIsMoving(false), bIsFlip(false), bIsCurrentFlipState(false), bIsBoosting(false), bIsDie(false), OriginalMaxSpeed(1200.f), BoostTime(0.f), MaxBoostTime(1.f)
	, LeanAngle(15.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerMesh = GetMesh();

	BikeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bike"));
	BikeMesh->SetupAttachment(GetMesh());
	BikeMesh->SetCollisionProfileName(TEXT("NoCollision"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<USoundBase> CoinSound(TEXT("/Game/GravityRacing/Audio/BGM/SW_Coin.SW_Coin"));
	if (CoinSound.Succeeded()) Coin = CoinSound.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> BoostSound(TEXT("/Game/GravityRacing/Audio/BGM/SW_Boost.SW_Boost"));
	if (BoostSound.Succeeded()) Boost = BoostSound.Object;


	PlayerMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	FRotator Rot = PlayerMesh->GetRelativeRotation();
	Rot.Yaw += -90.f;
	PlayerMesh->SetRelativeRotation(Rot);

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	GetCharacterMovement()->GravityScale = 4.f;

}

// Called when the game starts or when spawned
void AGRPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* controller = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(IMC, 0);
		}
	}

	GetCharacterMovement()->AirControl = 1.f;

	SetActorLocation(FVector(0.f, -OneLineDistance * 1.5, 0.f));
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AGRPlayer::HitEvent);
}

// Called every frame
void AGRPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsDie)
	{
		if (bIsMoving)
		{
			CrossLine(DeltaTime);
		}

		if (bIsFlip)
		{
			FlipPlayer(DeltaTime);
		}

		if (bIsBoosting)
		{
			BoostSpeedTime(DeltaTime);
		}

		if (bIsRecoverCenter)
		{
			RecoverCenter(DeltaTime);
		}

		AddMovementInput(FVector(1.f, 0.f, 0.f));
	}
	else
	{
		SetActorLocation(GetMesh()->GetComponentLocation() + FVector(0.f, 0.f, 100.f));
	}
}

// Called to bind functionality to input
void AGRPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGRPlayer::Move);
		input->BindAction(InGameMoveAction, ETriggerEvent::Triggered, this, &AGRPlayer::InGameMove);
		//input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGRPlayer::Look);
		input->BindAction(JumpAction, ETriggerEvent::Started, this, &AGRPlayer::JumpStart);
		input->BindAction(JumpAction, ETriggerEvent::Ongoing, this, &AGRPlayer::JumpStop);
		input->BindAction(FlipAction, ETriggerEvent::Started, this, &AGRPlayer::Flip);
	}

}

void AGRPlayer::Landed(const FHitResult& Hit)
{
	auto* movement = GetCharacterMovement();
	movement->SetMovementMode(EMovementMode::MOVE_Walking);
	movement->bOrientRotationToMovement = true;
	bIsFlip = false;
}

void AGRPlayer::HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	//AGRObstacle* Obstacle = Cast<AGRObstacle>(OtherActor);
	//if (Obstacle)
	//{
	//	bIsDie = true;

	//	FVector BounceDirection = SweepResult.ImpactNormal;
	//	BounceDirection.Normalize();

	//	float ImpulseStrength = 5000.0f;

	//	FVector FinalImpulse = BounceDirection * ImpulseStrength;

	//	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
	//	GetMesh()->AddImpulseToAllBodiesBelow(FinalImpulse, NAME_None);
	//	SpringArm->SetupAttachment(GetMesh());

	//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	GetMesh()->SetSimulatePhysics(true);
	//	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));

	//	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//	if (PC)
	//	{
	//		PC->SetInputMode(FInputModeUIOnly()); 
	//		//PC->bShowMouseCursor = true;          
	//	}

	//}
}

void AGRPlayer::SetPlayerMaxSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	bIsBoosting = true;
}

void AGRPlayer::BouncePlayer(const FHitResult& SweepResult)
{
	FVector BounceDirection = SweepResult.ImpactNormal;

	if (BounceDirection.X > -0.5f) BounceDirection.X = -0.5f;

	LaunchCharacter(BounceDirection * 2000.f + FVector(0, 0, 1000.f), true, true);
	GetCharacterMovement()->GravityScale *= 0.4;
	GRLOG("%s", *BounceDirection.ToString());
}

void AGRPlayer::Move(const FInputActionValue& value)
{
	if (bIsFlip) return;

	FVector2D input = value.Get<FVector2D>();

	FRotator rot = Controller->GetControlRotation();
	FRotator yaw(0.f, rot.Yaw, 0.f);

	FVector forward = FRotationMatrix(yaw).GetUnitAxis(EAxis::X);
	FVector right = FRotationMatrix(yaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(forward, input.Y);
	AddMovementInput(right, input.X);
}

void AGRPlayer::InGameMove(const FInputActionValue& value)
{
	if (bIsMoving || bIsFlip) return;

	float input = value.Get<float>();
	if (FMath::IsNearlyZero(input)) return;

	if (bIsCurrentFlipState)
	{
		input *= -1.f;
	}

	FRotator Rot = PlayerMesh->GetRelativeRotation();
	if (input > 0)
	{
		if (CurrentLine >= MAX_ROAD_LINE)
			return;

		TargetLocation = GetActorLocation() + GetActorRightVector() * OneLineDistance;
		++CurrentLine;

		Rot.Pitch = LeanAngle;
	}
	else if (input < 0)
	{
		if (CurrentLine <= 1)
			return;

		TargetLocation = GetActorLocation() - GetActorRightVector() * OneLineDistance;
		--CurrentLine;

		Rot.Pitch = -LeanAngle;
	}

	TargetRot = Rot;
	bIsRecoverCenter = false;
	bIsMoving = true;
}

void AGRPlayer::Look(const FInputActionValue& value)
{
	if (Controller == nullptr)
		return;

	FVector2D input = value.Get<FVector2D>();

	AddControllerYawInput(input.X);
	AddControllerPitchInput(input.Y);
}

void AGRPlayer::JumpStart(const FInputActionValue& value)
{
	Jump();
}

void AGRPlayer::JumpStop(const FInputActionValue& value)
{
	StopJumping();
}

void AGRPlayer::Flip(const FInputActionValue& value)
{
	if (bIsFlip) return;

	auto* movement = GetCharacterMovement();
	movement->SetMovementMode(EMovementMode::MOVE_Falling);

	movement->SetGravityDirection(FVector(0.f, 0.f, -movement->GetGravityDirection().Z));

	movement->bOrientRotationToMovement = false;

	PrevQuat = GetActorQuat();
	PitchDegree = RollDegree = 0.f;

	bIsFlip = true;
	bIsCurrentFlipState ^= 1;
	CurrentLine = MAX_ROAD_LINE - CurrentLine + 1;
}

void AGRPlayer::CrossLine(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 30.f);

	SetActorLocation(NewLocation);

	FRotator CurrentRotator = PlayerMesh->GetRelativeRotation();
	FRotator NewRotator = FMath::RInterpTo(CurrentRotator, TargetRot, DeltaTime, 30.f);
	PlayerMesh->SetRelativeRotation(NewRotator);

	if (abs(NewLocation.Y - TargetLocation.Y) < 10.f)
	{
		SetActorLocation(FVector(CurrentLocation.X, TargetLocation.Y, CurrentLocation.Z));
		FRotator Rot = PlayerMesh->GetRelativeRotation();
		TargetRot = FRotator(0.f, Rot.Yaw, Rot.Roll);
		bIsRecoverCenter = true;
		bIsMoving = false;
	}
}

void AGRPlayer::FlipPlayer(float DeltaTime)
{
	float PitchDelta = 180.f * DeltaTime * 2;
	float RollDelta = 360.f * DeltaTime * 2;

	PitchDegree += PitchDelta;
	RollDegree += RollDelta;

	FQuat QuatPitch = FQuat(FVector(1, 0, 0), FMath::DegreesToRadians(FMath::Min(PitchDegree, 180.f)));
	FQuat QuatRoll = FQuat(FVector(0, 1, 0), FMath::DegreesToRadians(FMath::Min(RollDegree, 360.f)));

	SetActorRotation(QuatPitch * QuatRoll * PrevQuat);
}

void AGRPlayer::BoostSpeedTime(float DeltaTime)
{
	BoostTime += DeltaTime;
	if (BoostTime >= MaxBoostTime)
	{
		GetCharacterMovement()->MaxWalkSpeed = OriginalMaxSpeed;
		bIsBoosting = false;
		BoostTime = 0.f;
	}
}

void AGRPlayer::RecoverCenter(float DeltaTime)
{
	FRotator CurrentRotator = PlayerMesh->GetRelativeRotation();
	FRotator NewRotator = FMath::RInterpTo(CurrentRotator, TargetRot, DeltaTime, 10.f);

	if (abs(NewRotator.Pitch - TargetRot.Pitch) < 1.f)
	{
		bIsRecoverCenter = false;
		NewRotator.Pitch = 0.f;
	}

	PlayerMesh->SetRelativeRotation(NewRotator);
}
