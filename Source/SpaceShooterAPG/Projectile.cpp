// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "SS_Player.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BulletComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletComponent"));
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));

	BulletComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CapsuleCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//ProjectileSpeed = 0.0f;

	bHit = false;
	bDestroy = false;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentLocation = this->GetActorLocation();

	//Here we bind the OnActorBeginOverlap to the AProjectile::BeginPlay()
	OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);


}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	//The projectile will be moved in the Tick
	Super::Tick(DeltaTime);

	FVector NewLocation = FVector(CurrentLocation.X + (ProjectileSpeed * DeltaTime), CurrentLocation.Y, CurrentLocation.Z);

	this->SetActorLocation(NewLocation);
	CurrentLocation = NewLocation;

	if (bHit)
		this->Destroy();
}

void AProjectile::OnBeginOverlap(AActor* ProjectileActor, AActor* OtherActor)
{

	//If we have collide with another actor.
	if (OtherActor) {
	
		ASS_Player* OwningPlayer = Cast<ASS_Player>(this->GetOwner());

		if (OtherActor->ActorHasTag(FName("Bounds"))) 
			bHit = true;

		if (this->ActorHasTag(FName("EnemyProjectile")) && OtherActor->ActorHasTag(FName("Player")))
			bHit = true;

		if (OtherActor->ActorHasTag(FName("EnemyShip"))) {
		
			if (OwningPlayer)
				OwningPlayer->PlayerScore += 50.0f;

			bHit = true;
		}

		if (OtherActor->ActorHasTag(FName("Asteroid"))) {

			if (OwningPlayer)
				OwningPlayer->PlayerScore += 10.0f;

			bHit = true;
		}

	}
}