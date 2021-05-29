// Fill out your copyright notice in the Description page of Project Settings.


#include "SS_Player.h"

// Sets default values
ASS_Player::ASS_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	ExplosionFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionComponent"));
	DeathExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathExplosionSound"));

	ShipMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ParticleSystem->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionFX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DeathExplosionSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	MaxVelocity = 250.0f;
	Current_X_Velocity = 0.0f;
	Current_Y_Velocity = 0.0f;

	BIsFiring = false;
	WeaponFireRate = 0.25f; //Every quater of a second the player will be allowed to shoot. Note: the Tick function runs at 60fps.
	TimeSinceLastShot = 0.0f;

	PlayerScore = 0.0f;
}

// Called when the game starts or when spawned
void ASS_Player::BeginPlay()
{
	Super::BeginPlay();
	
	Current_Location = this->GetActorLocation();
	Current_Rotation = this->GetActorRotation();

	bHit = false;
	bDead = false;

	ExplosionFX->Deactivate();
	DeathExplosionSound->Deactivate();

	Max_Health = 100.0f;
	Current_Health = 100.0f;

	Max_Armor = 100.0f;
	Current_Armor = 100.0f;

	OnActorBeginOverlap.AddDynamic(this, &ASS_Player::OnBeginOverlap);

}

// Called every frame
void ASS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Current_X_Velocity != 0.0f || Current_Y_Velocity != 0.0f) {
		
		New_Location = FVector(Current_Location.X + (Current_X_Velocity * DeltaTime), 
			Current_Location.Y + (Current_Y_Velocity * DeltaTime), 0);

		this->SetActorLocation(New_Location);

		Current_Location = New_Location;
	}

	if (Current_Y_Velocity > 0.1f) {
	
		this->SetActorRotation(FRotator(Current_Rotation.Pitch + 45.0f, Current_Rotation.Yaw, Current_Rotation.Roll));
	}
	else if (Current_Y_Velocity < -0.1f) {
	
		this->SetActorRotation(FRotator(Current_Rotation.Pitch - 45.0f, Current_Rotation.Yaw, Current_Rotation.Roll));
	}
	else {
	
		this->SetActorRotation(FRotator(Current_Rotation));
	}

	if (this->GetActorLocation().X > Field_Width)
		Current_Location = FVector(Field_Width - 1, Current_Location.Y, Current_Location.Z);

	if (this->GetActorLocation().X < -Field_Width)
		Current_Location = FVector(-Field_Width + 1, Current_Location.Y, Current_Location.Z);

	if (this->GetActorLocation().Y > Field_Height)
		Current_Location = FVector(Current_Location.X, Field_Height - 1, Current_Location.Z);

	if (this->GetActorLocation().Y < -Field_Height)
		Current_Location = FVector(Current_Location.X, -Field_Height + 1, Current_Location.Z);
	
	//handle Firing. Here we are testing if the player is firing.
	if (BIsFiring) {
	
		if (TimeSinceLastShot > WeaponFireRate) {

			FireWeapon();
			TimeSinceLastShot = 0.0f;
		}
	}

	TimeSinceLastShot += DeltaTime;
	//TimeSinceLastShot = TimeSinceLastShot + DeltaTime; Note: this is what the above line means.

}//tick end

// Called to bind functionality to input
void ASS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Binding the move controls
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASS_Player::MoveRight);
	PlayerInputComponent->BindAxis(FName("MoveUp"), this, &ASS_Player::MoveUp);

	//Binding the shoot controls
	PlayerInputComponent->BindAction(FName("Fire"), IE_Pressed, this, &ASS_Player::StartFiring);
	PlayerInputComponent->BindAction(FName("Fire"), IE_Released, this, &ASS_Player::StopFiring);
}

void ASS_Player::MoveRight(float AxisValue)
{
	Current_X_Velocity = MaxVelocity * AxisValue;
}

void ASS_Player::MoveUp(float AxisValue)
{
	Current_Y_Velocity = MaxVelocity * AxisValue;
}

void ASS_Player::StartFiring()
{
	BIsFiring = true;
}

void ASS_Player::StopFiring()
{
	BIsFiring = false;
}

//The player projectile is spawned from here.
void ASS_Player::FireWeapon()
{
	FActorSpawnParameters Params = {};
	Params.Owner = this; //Telling the projectile who it's owner is.

	//The projectile actor is spawned in the world from here. It is spawned from the location of the player.
	AActor* SpawnedProjectile = GetWorld()->SpawnActor(WeaponProjectile_BP, &Current_Location, &Current_Rotation, Params);
}

void ASS_Player::CollectablePickup() {

	UE_LOG(LogTemp, Warning, TEXT("PICKUP HAS BEEN COLLECTED"))
}

void ASS_Player::OnBeginOverlap(AActor* PlayerActor, AActor* OtherActor)
{

}