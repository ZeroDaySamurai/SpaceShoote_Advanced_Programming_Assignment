// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "Enemy.h"
#include "Hazard.h"
#include "Blueprint/UserWidget.h"

#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTERAPG_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		TSubclassOf<AEnemy> Enemy;

		UPROPERTY(EditAnywhere)
			TSubclassOf<AHazard> HazardTemplate;

		UPROPERTY(EditAnywhere)
			UAudioComponent* MusicTrack;

		//Constructor
		AMyGameMode();

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime)override;

		TSubclassOf<AHazard> GetHazardTemplate();

public:

	int32 SpawnTimer;
	bool bPlayerDead;

	UFUNCTION()
		void RestartLevel(FName LevelName);

	bool bStart;

protected:

	FActorSpawnParameters AsteroidSpawnParameters;
	FVector AsteroidSpawnLoc;
	FRotator AsteroidSpawnRot;

	UWorld* ThisWorld;

	//Add Widgets

	//The Score User Interface Widget
	UPROPERTY(EditAnywhere, Category = "HUD ELements")
		TSubclassOf<UUserWidget> Score_Widget_Class;
	UUserWidget* Score_Widget;

	//The Health and Armor User Interface Widget
	UPROPERTY(EditAnywhere, Category = "HUD ELements")
		TSubclassOf<UUserWidget> Shield_And_Armor_Widget_Class;
	UUserWidget* Shield_Armor_Widget;

	//The Restart User Interface Widget
	UPROPERTY(EditAnywhere, Category = "HUD ELements")
		TSubclassOf<UUserWidget> Restart_Widget_Class;
	UUserWidget* Restart_Widget;

	APlayerController* PC_Ref;
};
