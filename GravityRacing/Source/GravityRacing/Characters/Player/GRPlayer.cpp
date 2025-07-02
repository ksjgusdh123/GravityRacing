// Fill out your copyright notice in the Description page of Project Settings.


#include "GRPlayer.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
AGRPlayer::AGRPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	GravityArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("GravityArrow"));
	GravityArrow->SetupAttachment(RootComponent);
	GravityArrow->SetRelativeLocation(FVector(0.f, 0.f, -80.f));
	GravityArrow->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GravityArrow->SetArrowColor(FColor::Green);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Assets/Player/Mesh/Skin_1/SK_Skin_1.SK_Skin_1"));

	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}
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
}

// Called every frame
void AGRPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AGRPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGRPlayer::Move);
		input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGRPlayer::Look);
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
	IsFlip = false;
}

void AGRPlayer::Move(const FInputActionValue& value)
{
	if (Controller == nullptr)
		return;

	FVector2D input = value.Get<FVector2D>();

	FRotator rot = Controller->GetControlRotation();
	FRotator yaw(0.f, rot.Yaw, 0.f);

	FVector forward = FRotationMatrix(yaw).GetUnitAxis(EAxis::X);
	FVector right = FRotationMatrix(yaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(forward, input.Y);
	AddMovementInput(right, input.X);
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
	auto* movement = GetCharacterMovement();
	movement->SetMovementMode(EMovementMode::MOVE_Falling);

	movement->SetGravityDirection(FVector(0.f, 0.f, movement->GetGravityDirection().Z * -1));

	movement->bOrientRotationToMovement = false;
	movement->Velocity = FVector(0.f, 0.f, 0.f);

	FRotator ReverseRot = GetActorRotation();
	ReverseRot.Roll += 180.f;
	SetActorRotation(FRotator(ReverseRot));

	IsFlip = true;
}

