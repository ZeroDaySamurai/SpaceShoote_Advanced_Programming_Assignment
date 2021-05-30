// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"

//Constructor
AMyGameMode::AMyGameMode() {

	SpawnTimer = 0.0f;

	MusicTrack = CreateAbstractDefaultSubobject<UAudioComponent>(TEXT("Music"));
	MusicTrack->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	AsteroidSpawnLoc = FVector(1000.0f, 1000.0f, 1000.0f);
	AsteroidSpawnRot = FRotator(0.0f, 0.0f, 0.0f);

	ThisWorld = GetWorld();
}

void AMyGameMode::BeginPlay()
{

	Super::BeginPlay();

	MusicTrack->Play();

	bPlayerDead = false;



}

void AMyGameMode::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	SpawnTimer = FMath::RandRange(0, 1000);
	
	if (Enemy) {
	
		if (SpawnTimer > 990) {
	
			FVector EnemySpawnLoc(0.0f, 0.0f, 0.0f);
			FRotator EnemySpawnRot(0.0f, 0.0f, 0.0f);

			AActor* NewEnemy = GetWorld()->SpawnActor(Enemy, &EnemySpawnLoc, &EnemySpawnRot, AsteroidSpawnParameters);

			if (NewEnemy) 
				NewEnemy->SetActorLocation(FVector(1000.0f, FMath::RandRange(-500, 500), 0.0f));
		
		}
	}//Spawn Enemy

	if (HazardTemplate) {

		if (SpawnTimer > 995){
		
			AActor* NewHazard = GetWorld()->SpawnActor(HazardTemplate, &AsteroidSpawnLoc, &AsteroidSpawnRot, AsteroidSpawnParameters);

			if (NewHazard)
				NewHazard->SetActorLocation(FVector(1000.0f, 1000.0f, 1000.0f));
		}
	}

}//Tick

void AMyGameMode::RestartLevel(FName LevelName)
{
}
