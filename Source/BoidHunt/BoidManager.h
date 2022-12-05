// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelBuilder.h"
#include "GameFramework/Actor.h"
#include "BoidManager.generated.h"

class ABoid;
class AFalcon;
UCLASS()
class BOIDHUNT_API ABoidManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoidManager();
	FBounds3d GetBounds() const;
	// Returns a MultipMap of Boids divided by their PartitionKey.
	const TMultiMap<FIntVector2, ABoid*>* GetBoidMap() const;
	const TArray<AFalcon*>* ABoidManager::GetFalcons() const;
	// Returns a PartitionKey for the multimap.
	FIntVector2 GetPartitionKeyFromLocation(FVector Location) const;
	// Returns all Boids.
	TArray<ABoid*>* GetBoidArray();
	// Spawns a Boid and adds it do Manager's data structures.
	void SpawnBoid(FVector Location);
	// Spawns a Falcon and adds it do Manager's data structures.
	void SpawnFalcon(FVector Location, FVector Direction);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	// Called from BeginPlay(), spawns (BoidCount) Boids.
	void SpawnBoids();

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABoid> BoidBlueprint;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AFalcon> FalconBlueprint;
	// Size of one spatial partitioning sector, shouldn't be less than the FlockingRadius of Boids.
	UPROPERTY(EditAnywhere)
	double SpatialPartitionSize;
	// Number of Boids to spawn in SpawnBoids().
	UPROPERTY(EditAnywhere)
	int BoidCount;
	// Dimensions of the box that boids will spawn in
	UPROPERTY(EditAnywhere)
	FVector BoxDimensions;
	UPROPERTY()
	TArray<ABoid*> BoidArray;
	UPROPERTY()
	TArray<AFalcon*> Falcons;
	UPROPERTY(EditInstanceOnly)
	ALevelBuilder* LevelBuilder;
	UPROPERTY()
	USceneComponent* SceneComponent;
	TMultiMap<FIntVector2, ABoid*> BoidMap;
};
