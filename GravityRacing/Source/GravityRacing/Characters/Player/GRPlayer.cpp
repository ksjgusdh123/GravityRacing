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
#include <Kismet/GameplayStatics.h>
#include <Sound/SoundCue.h>

// Sets default values
AGRPlayer::AGRPlayer()
	: RoadDistance(380.f), CurrentLine(1), bIsMoving(false), bIsFlip(false), bIsCurrentFlipState(false), bIsBoosting(false), OriginalMaxSpeed(600.f), BoostTime(0.f), MaxBoostTime(1.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	FRotator Rot = GetMesh()->GetRelativeRotation();
	Rot.Yaw += -90.f;
	GetMesh()->SetRelativeRotation(Rot);

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
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
	
	SetActorLocation(FVector(0.f, -RoadDistance * 1.5, 0.f));
}

// Called every frame
void AGRPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	FQuat NewQuat = GetActorQuat();

	AddMovementInput(FVector(1.f, 0.f, 0.f));
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

void AGRPlayer::SetPlayerMaxSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	bIsBoosting = true;
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

	if (input > 0)
	{
		if (CurrentLine >= MAX_ROAD_LINE)
			return;

		TargetLocation = GetActorLocation() + GetActorRightVector() * RoadDistance;
		++CurrentLine;
	}
	else if (input < 0)
	{
		if (CurrentLine <= 1)
			return;

		TargetLocation = GetActorLocation() - GetActorRightVector() * RoadDistance;
		--CurrentLine;
	}

		
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

	movement->SetGravityDirection(FVector(0.f, 0.f, movement->GetGravityDirection().Z * -1));

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

	if (abs(NewLocation.Y - TargetLocation.Y) < 10.f)
	{
		SetActorLocation(FVector(CurrentLocation.X, TargetLocation.Y, CurrentLocation.Z));
		bIsMoving = false;
	}
}

void AGRPlayer::FlipPlayer(float DeltaTime)
{
	float PitchDelta = 180.f * DeltaTime;
	float RollDelta = 360.f * DeltaTime;

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

void AGRPlayer::PlayMusic(EGameSound SoundType)
{
	switch (SoundType) {
	case EGameSound::Coin:
		UGameplayStatics::PlaySound2D(this,Coin);
		break;

	case EGameSound::Boost:
		UGameplayStatics::PlaySound2D(this, Boost);
		break;

	}
	
}
