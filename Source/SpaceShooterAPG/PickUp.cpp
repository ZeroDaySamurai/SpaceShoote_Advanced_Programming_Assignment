// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"
#include "SS_Player.h"

// Sets default values
APickUp::APickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	CollisionMesh = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Component"));
	 
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &APickUp::OnBeginOverlap);
	
}

// Called every frame
void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->SetActorLocation(FVector(this->GetActorLocation().X - (DeltaTime * 100.0f), this->GetActorLocation().Y, 0.0f));

}

void APickUp::OnBeginOverlap(AActor * PickupActor, AActor * OtherActor) {

	if (OtherActor->ActorHasTag("Bounds")){
	
		ASS_Player* PlayerActor = Cast<ASS_Player>(OtherActor);
		PlayerActor->CollectablePickup();

		this->Destroy();
	}
	
	if (OtherActor->ActorHasTag("Bounds"))
		this->Destroy();

}