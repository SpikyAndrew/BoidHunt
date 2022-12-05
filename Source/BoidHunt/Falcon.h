// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlyerBase.h"
#include "Falcon.generated.h"

/**
 * 
 */
UCLASS()
class BOIDHUNT_API AFalcon : public AFlyerBase
{
	GENERATED_BODY()
public:
	AFalcon();
	virtual void BeginPlay() override;
	virtual void Deactivate() override;
	// Sets Velocity and BoidManager
	void Initialize(ABoidManager* Manager, const FVector& Direction);
protected:
	virtual void Tick(float DeltaSeconds) override;
	// Eats the Boid that was hit (if that's what was hit).
	UFUNCTION()
	void OnHitCheckForBoid(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor,
	                       UPrimitiveComponent* OtherComponent, FVector Vector, const FHitResult& HitResult);
	// Steers towards the nearest Boid with SteeringStrength, unless NoSteeringTimer hasn't reached zero yet.
	virtual void SteerTowardsGoals(float DeltaTime) override;
	// Returns the difference between current position and the position of nearest Boid.
	FVector GetRelativePositionOfPrey() const;

	UPROPERTY(EditAnywhere)
	double StartingEnergy;
	UPROPERTY(EditAnywhere)
	double EnergyGainPerKill;
	UPROPERTY(EditAnywhere)
	double SteeringStrength;
	// Initial time for which the Falcon doesn't perform any steering.
	UPROPERTY(EditAnywhere)
	double NoSteeringTimer;

	FVector PreviousVelocity;
	// Used up when steering.
	double Energy;
};
