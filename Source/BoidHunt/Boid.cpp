// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"

#include "BoidHuntGameState.h"
#include "BoidManagerSubsystem.h"
#include "GameFramework/GameStateBase.h"
#include "Falcon.h"

ABoid::ABoid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentHit.AddDynamic(this, &AFlyerBase::BounceOnHit);
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
			DrawDebugLine(GetWorld(), GetActorLocation(), Falcon->GetActorLocation(), FColor::Green, false, -1, 0, 5);
		}
	}

	return IsAvoidingFalcons;
}

void ABoid::SteerTowardsGoals(float DeltaTime)
{
	Super::SteerTowardsGoals(DeltaTime);

	bool IsAvoidingFalcons = AvoidFalcons(DeltaTime);

	if (!IsAvoidingFalcons)
	{
		FIntVector2 PartitionKey = BoidManager->GetPartitionKeyFromLocation(GetActorLocation());
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
	// Find the center of the nearby flock's mass.
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
