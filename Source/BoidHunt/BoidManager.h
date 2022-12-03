// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boid.h"
#include "GridSpawner.h"
#include "GameFramework/Actor.h"
#include "BoidManager.generated.h"

class AFalcon;
UCLASS()
class BOIDHUNT_API ABoidManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidManager();
	const TArray<const ABoid*>* GetBoids() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SpawnBoids();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABoid> BoidBlueprint;
	UPROPERTY(EditAnywhere)
	int BoidCount;
	UPROPERTY()
	TArray<const ABoid*> Boids;
	// Dimensions of the box that boids will spawn in
	UPROPERTY(EditAnywhere)
	FVector BoxDimensions;
	UPROPERTY(EditAnywhere)
	TArray<AFalcon*> Falcons;
	UPROPERTY()
	USceneComponent* SceneComponent;
};
