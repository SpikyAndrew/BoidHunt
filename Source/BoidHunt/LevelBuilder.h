// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Bounds.h"
#include "LevelBuilder.generated.h"

UCLASS(Config=Game)
class BOIDHUNT_API ALevelBuilder : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelBuilder();
	// Returns max height for flying boids.
	double GetMaxHeight() const;
	FBounds3d CalculateLevelBounds() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FBounds3d GetBounds() const;

protected:
	// Prints a warning message on screen.
	static void PrintValidationWarning(FString Text);
	// Fixes errors caused by bad config and shows debug messages about it.
	void ValidateAndFixConfig();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Spawns buildings.
	void Spawn();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorToSpawn;
	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;
	// Space between building spawn points on the grid.
	UPROPERTY(EditAnywhere)
	double CellSize;
	// Horizontal scale applied to buildings.
	UPROPERTY(EditAnywhere)
	double BuildingWidth;
	// "Soft" bounds for all flyers. They will prioritise maintaining these.
	UPROPERTY(EditAnywhere)
	double BoidMinimumAltitude;
	UPROPERTY(EditAnywhere)
	double BoidMaximumAltitudeAboveTallestBuilding;
	UPROPERTY(EditAnywhere)
	double BuildingMeshHeight;
	// Dimensions of the building grid.
	UPROPERTY(Config)
	int Width;
	UPROPERTY(Config)
	int Length;
	// Vertical scaling to apply to buildings. Use as a two-dimensional array.
	UPROPERTY(Config)
	TArray<double> BuildingHeights;

	FBounds3d Bounds;

	const int MinLevelSize = 1;
};
