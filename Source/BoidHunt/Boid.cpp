// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"

#include "BoidHuntGameState.h"
#include "GameFramework/GameStateBase.h"

ABoid::ABoid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Sets default values

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentHit.AddUniqueDynamic(this, &AFlyerBase::BounceOnHit);
}

void ABoid::Deactivate()
{
	IsAlive = false;
	SetActorTickEnabled(false);
	Mesh->SetVisibility(false);
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABoid::SteerTowardsGoals(float DeltaTime)
{
	Super::SteerTowardsGoals(DeltaTime);

	ApplySeparationRule(DeltaTime);
	ApplyAlignmentRule(DeltaTime);
	ApplyCohesionRule(DeltaTime);
}

void ABoid::ApplySeparationRule(float DeltaTime)
{
	for (const ABoid* Other : *BoidsArray)
	{
		if (Other != this && Other->GetIsAlive())
		{
			FVector LocationDifference = Other->GetActorLocation() - GetActorLocation();
			const double Distance = LocationDifference.Length();
			if (Distance < SeparationRadius)
			{
				 Velocity -= LocationDifference * DeltaTime * SeparationStrength;
			}
		}
	}
}

void ABoid::ApplyAlignmentRule(float DeltaTime)
{
	FVector AverageVelocity = FVector::Zero();
	int OthersInCohesion = 0;

	for (const ABoid* Other : *BoidsArray)
	{
		if (Other != this)
		{
			FVector LocationDifference = Other->GetActorLocation() - GetActorLocation();
			const double Distance = LocationDifference.Length();

			if (Distance < FlockingRadius)
			{
				AverageVelocity += Other->Velocity;
				OthersInCohesion++;
			}
		}
	}
	
	if (OthersInCohesion == 0)
		return;
	
	AverageVelocity /= OthersInCohesion;
	AverageVelocity -= Velocity / 8;
	Velocity += AverageVelocity * DeltaTime * AlignmentStrength;
}

void ABoid::ApplyCohesionRule(float DeltaTime)
{
	// Find the center of the nearby flock's mass.
	FVector Center = FVector::Zero();
	int OthersInCohesion = 0;
	for (const ABoid* Other : *BoidsArray)
	{
		if (Other != this)
		{
			FVector LocationDifference = Other->GetActorLocation() - GetActorLocation();
			const double Distance = LocationDifference.Length();

			if (Distance < FlockingRadius)
			{
				Center += Other->GetActorLocation();
				OthersInCohesion++;
			}
		}
	}

	if (OthersInCohesion == 0)
		return;
	Center /= OthersInCohesion;
	Velocity += (Center - GetActorLocation()) * DeltaTime * CohesionStrength;
}