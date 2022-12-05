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
	const TMultiMap<FIntVector2, ABoid*>* GetBoidMap() const;
	const TArray<AFalcon*>* ABoidManager::GetFalcons() const;
	FIntVector2 GetPartitionKeyFromLocation(FVector Location) const;
	void SpawnBoid(FVector Location);
	void SpawnFalcon(FVector Location, FVector Direction);
	TArray<ABoid*>* GetBoidArray();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void SpawnBoids();

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABoid> BoidBlueprint;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AFalcon> FalconBlueprint;
	UPROPERTY(EditAnywhere)
	double SpatialPartitionSize;
	UPROPERTY(EditAnywhere)
	int BoidCount;
	UPROPERTY()
	TArray<AFalcon*> Falcons;
	UPROPERTY(EditInstanceOnly)
	ALevelBuilder* LevelBuilder;
	// Dimensions of the box that boids will spawn in
	UPROPERTY(EditAnywhere)
	FVector BoxDimensions;
	UPROPERTY()
	USceneComponent* SceneComponent;
	UPROPERTY()
	TArray<ABoid*> BoidArray;
	TMultiMap<FIntVector2, ABoid*> BoidMap;
};
