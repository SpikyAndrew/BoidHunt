// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"

#include "BoidHuntGameState.h"
#include "Falcon.h"

ABoid::ABoid()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABoid::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentHit.AddUniqueDynamic(this, &AFlyerBase::BounceOnHit);
}

bool ABoid::AvoidFalcons(double DeltaTime)
{
	bool IsAvoidingFalcons = false;

	for (AFalcon* Falcon : *BoidManager->GetFalcons())
	{
		if (!Falcon->GetIsAlive())
		{
			continue;
		}

		FVector LocationDiff = Falcon->GetActorLocation() - GetActorLocation();

		if (LocationDiff.Length() < FalconAvoidanceRadius)
		{
			LocationDiff.Normalize();
			Velocity -= LocationDiff * DeltaTime * FalconAvoidanceStrength;
			IsAvoidingFalcons = true;
		}
	}

	return IsAvoidingFalcons;
}

void ABoid::SteerTowardsGoals(float DeltaTime)
{
	Super::SteerTowardsGoals(DeltaTime);

	const bool IsAvoidingFalcons = AvoidFalcons(DeltaTime);

	if (!IsAvoidingFalcons)
	{
		const FIntVector2 PartitionKey = BoidManager->GetPartitionKeyFromLocation(GetActorLocation());
		TMultiMap<FIntVector2, ABoid*> BoidMap = *BoidManager->GetBoidMap();

		ApplySeparationRule(DeltaTime, BoidMap, PartitionKey);
		ApplyAlignmentRule(DeltaTime, BoidMap, PartitionKey);
		ApplyCohesionRule(DeltaTime, BoidMap, PartitionKey);
	}
}

void ABoid::ApplySeparationRule(float DeltaTime, TMultiMap<FIntVector2, ABoid*> BoidMap, FIntVector2 PartitionKey)
{

	for (FIntVector2 Direction : Directions)
	{
		FIntVector2 Key = FIntVector2(PartitionKey.X + Direction.X, PartitionKey.Y + Direction.Y);
		TArray<ABoid*> BoidArray;
		BoidMap.MultiFind(Key, BoidArray);
		for (const ABoid* Other : BoidArray)
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
}

void ABoid::ApplyAlignmentRule(float DeltaTime, TMultiMap<FIntVector2, ABoid*> BoidMap, FIntVector2 PartitionKey)
{
	FVector AverageVelocity = FVector::Zero();
	int OthersInCohesion = 0;

	// Find the average velocity of Boids in FlockingRadius.
	for (FIntVector2 Direction : Directions)
	{
		FIntVector2 Key = FIntVector2(PartitionKey.X + Direction.X, PartitionKey.Y + Direction.Y);
		TArray<ABoid*> BoidArray;
		BoidMap.MultiFind(Key, BoidArray);
		for (const ABoid* Other : BoidArray)
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
	}

	if (OthersInCohesion == 0)
	{
		return;
	}

	AverageVelocity /= OthersInCohesion;
	AverageVelocity -= Velocity / 8;
	Velocity += AverageVelocity * DeltaTime * AlignmentStrength;
}

void ABoid::ApplyCohesionRule(float DeltaTime, TMultiMap<FIntVector2, ABoid*> BoidMap, FIntVector2 PartitionKey)
{
	// Find the center of mass for all boids within FlockingRadius.
	FVector Center = FVector::Zero();
	int OthersInCohesion = 0;
	for (FIntVector2 Direction : Directions)
	{
		FIntVector2 Key = FIntVector2(PartitionKey.X + Direction.X, PartitionKey.Y + Direction.Y);
		TArray<ABoid*> BoidArray;
		BoidMap.MultiFind(Key, BoidArray);
		for (const ABoid* Other : BoidArray)
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
	}
	
	if (OthersInCohesion == 0)
	{
		return;
	}
	Center /= OthersInCohesion;
	Velocity += (Center - GetActorLocation()) * DeltaTime * CohesionStrength;
}

void ABoid::Deactivate()
{
	Super::Deactivate();
	if (UWorld* World = GetWorld())
	{
		ABoidHuntGameState* GameState = World->GetGameState<ABoidHuntGameState>();
		if (GameState)
		{
			GameState->AddBoids(-1);
		}
	}
}
