// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlyerBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

UCLASS()
class BOIDHUNT_API ABoid : public AFlyerBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();
	void BeginPlay();

protected:
	virtual void SteerTowardsGoals(float DeltaTime) override;
	bool AvoidFalcons(double DeltaTime);
	void ApplySeparationRule(float DeltaTime);
	void ApplyAlignmentRule(float DeltaTime);
	void ApplyCohesionRule(float DeltaTime);

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
