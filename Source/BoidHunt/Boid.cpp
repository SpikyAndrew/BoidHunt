// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"

#include "BoidHuntGameState.h"
#include "GameFramework/GameStateBase.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();

	Velocity = FMath::VRand() * 1000;
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BoidsArray)
	{
		ApplySeparationRule(DeltaTime);
		ApplyAlignmentRule(DeltaTime);
		ApplyCohesionRule(DeltaTime);
	}

	StayInBounds(DeltaTime);
	double MaxVelocity = FMath::Lerp(MaxVelocityDownwards, MaxVelocityUpwards,(1 + Velocity.GetClampedToSize(0,1).Dot(FVector::UpVector))/2);
	Velocity = Velocity.GetClampedToSize(0,MaxVelocity);
	MoveWithVelocity(DeltaTime);
	LookForward();
}

// Sets BoidsArray if it hasn't been set to anything yet.
void ABoid::Initialize(const TArray<const ABoid*>* Boids)
{
	if (!BoidsArray)
		BoidsArray = Boids;
}

void ABoid::ApplySeparationRule(float DeltaTime)
{
	for (const ABoid* Other : *BoidsArray)
	{
		if (Other != this)
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

void ABoid::StayInBounds(float DeltaTime)
{
	const FVector Location = GetActorLocation();

	if (Location.X < MinBounds.X)
		Velocity += FVector::ForwardVector * BoundsStrength * DeltaTime;
	else if (Location.X > MaxBounds.X)
		Velocity -= FVector::ForwardVector * BoundsStrength * DeltaTime;


	if (Location.Y < MinBounds.Y)
		Velocity += FVector::RightVector * BoundsStrength * DeltaTime;
	else if (Location.Y > MaxBounds.Y)
		Velocity -= FVector::RightVector * BoundsStrength * DeltaTime;

	
	if (Location.Z < MinBounds.Z)
		Velocity += FVector::UpVector * BoundsStrength * DeltaTime;
	else if (Location.Z > MaxBounds.Z)
		Velocity -= FVector::UpVector * BoundsStrength * DeltaTime;
}

void ABoid::MoveWithVelocity(float DeltaTime)
{
	AddActorWorldOffset(Velocity * DeltaTime);
}

void ABoid::LookForward()
{
	SetActorRotation(Velocity.Rotation().Add(-90,0,0));
}

