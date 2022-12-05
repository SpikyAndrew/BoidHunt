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
	UFUNCTION()
	void OnHitCheckForBoid(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector Vector, const FHitResult& HitResult);
	virtual void BeginPlay() override;
	virtual void Deactivate() override;
	void Initialize(ABoidManager* Manager, const FVector& Direction);
protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual	void SteerTowardsGoals(float DeltaTime) override;
	FVector GetRelativePositionOfPrey() const;

	UPROPERTY(EditAnywhere)
	double StartingEnergy;
	UPROPERTY(EditAnywhere)
	double EnergyGainPerKill;
	UPROPERTY(EditAnywhere)
	double SteeringStrength;
	UPROPERTY(EditAnywhere)
	double NoSteeringTimer;
	
	FVector PreviousVelocity;
	double Energy;
};
