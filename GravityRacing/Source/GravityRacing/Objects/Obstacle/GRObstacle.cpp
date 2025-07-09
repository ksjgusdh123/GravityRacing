// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Obstacle/GRObstacle.h"
#include "Components/BoxComponent.h"

// Sets default values
AGRObstacle::AGRObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);

	CollisionBox->SetNotifyRigidBodyCollision(true);
	CollisionBox->SetCollisionProfileName(TEXT("BlockAll"));
	CollisionBox->OnComponentHit.AddDynamic(this, &AGRObstacle::HitEvent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CollisionBox);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/EditorMeshes/EditorCube.EditorCube"));

	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void AGRObstacle::BeginPlay()
{
	Super::BeginPlay();
	
	UStaticMeshComponent* mesh = FindComponentByClass<UStaticMeshComponent>();
	UBoxComponent* box = FindComponentByClass<UBoxComponent>();

	//if (mesh && box)
	//{
	//	FVector extent, origin;
	//	mesh->GetLocalBounds(origin, extent);

	//	box->SetBoxExtent(extent);
	//	box->SetRelativeLocation(FVector(origin));
	//}
}

// Called every frame
void AGRObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGRObstacle::HitEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("NOTIFYOVERRIDE"));
	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("hit"), *OtherActor->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Msg);
	}
}

