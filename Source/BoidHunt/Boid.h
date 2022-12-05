// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlyerBase.h"
#include "Boid.generated.h"

UCLASS()
class BOIDHUNT_API ABoid : public AFlyerBase
{
	GENERATED_BODY()

public:
	ABoid();
	// Called when Boid is killed.
	virtual void Deactivate() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Steers based on Boids algorithm.
	virtual void SteerTowardsGoals(float DeltaTime) override;
	// Looks for Falcons in FalconAvoidanceRadius, then avoids them with FalconAvoidanceStrength.
	bool AvoidFalcons(double DeltaTime);
	// Looks for Boids in SeparationRadius, then avoids them with SeparationStrength.
	void ApplySeparationRule(float DeltaTime, TMultiMap<FIntVector2, ABoid*> BoidMap, FIntVector2 PartitionKey);
	// Looks for Boids in FlockingRadius, then aligns with their average velocity with AlignmentStrength.
	void ApplyAlignmentRule(float DeltaTime, TMultiMap<FIntVector2, ABoid*> BoidMap, FIntVector2 PartitionKey);
	// Looks for Boids in FlockingRadius, then steers towards them with CohesionStrength.
	void ApplyCohesionRule(float DeltaTime, TMultiMap<FIntVector2, ABoid*> BoidMap, FIntVector2 PartitionKey);

	UPROPERTY(EditAnywhere)
	double SeparationStrength;
	UPROPERTY(EditAnywhere)
	double AlignmentStrength;
	UPROPERTY(EditAnywhere)
	double CohesionStrength;
	UPROPERTY(EditAnywhere)
	double FlockingRadius;
	UPROPERTY(EditAnywhere)
	double SeparationRadius;
	UPROPERTY(EditAnywhere)
	double FalconAvoidanceRadius;
	UPROPERTY(EditAnywhere)
	double FalconAvoidanceStrength;
};
