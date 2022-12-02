// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"

#include "BoidHuntGameState.h"
#include "GameFramework/GameStateBase.h"

ABoid::ABoid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));

	RootComponent = Collider;
	Mesh->SetupAttachment(Collider);
	Mesh->SetRelativeLocation(FVector::ZeroVector);
}

// Sets default values

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();

	Velocity = FMath::VRand() * MaxVelocityDownwards;
	Collider->OnComponentHit.AddUniqueDynamic(this, &ABoid::OnHit);
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool IsAvoiding =  AvoidObstacles(DeltaTime);
	bool IsOutOfBounds = StayInBounds(DeltaTime);

	if (BoidsArray && !IsAvoiding && !IsOutOfBounds)
	{
		ApplySeparationRule(DeltaTime);
		ApplyAlignmentRule(DeltaTime);
		ApplyCohesionRule(DeltaTime);
	}

	
	double MaxVelocity = FMath::Lerp(MaxVelocityDownwards, MaxVelocityUpwards,
		(1 + Velocity.GetClampedToSize(1,1).Dot(FVector::UpVector))/2);
	Velocity = Velocity.GetClampedToSize(MinVelocity,MaxVelocity);
	MoveWithVelocity(DeltaTime);
	LookForward();
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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

bool ABoid::StayInBounds(float DeltaTime)
{
	const FVector Location = GetActorLocation();

	bool IsOutOfBounds = false;
	
	if (Location.X < MinBounds.X)
	{
		IsOutOfBounds = true;
		Velocity += FVector::ForwardVector * BoundsStrength * DeltaTime;
	}
	else if (Location.X > MaxBounds.X)
	{
		IsOutOfBounds = true;
	Velocity -= FVector::ForwardVector * BoundsStrength * DeltaTime;
	}


	if (Location.Y < MinBounds.Y)
	{
		IsOutOfBounds = true;
		Velocity += FVector::RightVector * BoundsStrength * DeltaTime;
	}
	else if (Location.Y > MaxBounds.Y)
	{
		IsOutOfBounds = true;
		Velocity -= FVector::RightVector * BoundsStrength * DeltaTime;
	}

	
	if (Location.Z < MinBounds.Z)
	{
		IsOutOfBounds = true;
		Velocity += FVector::UpVector * BoundsStrength * DeltaTime;
	}
	else if (Location.Z > MaxBounds.Z)
	{
		IsOutOfBounds = true;
		Velocity -= FVector::UpVector * BoundsStrength * DeltaTime;
	}

	return IsOutOfBounds;
}

void ABoid::MoveWithVelocity(float DeltaTime)
{
	AddActorWorldOffset(Velocity * DeltaTime, true);
}

bool ABoid::AvoidObstacles(float DeltaTime)
{
	FHitResult HitDetails;
	const FVector Start = GetActorLocation();
	const FVector End = Start + Velocity * BuildingAvoidanceEagerness;
	FCollisionQueryParams TraceParams;
	bool bIsHit = GetWorld()->SweepSingleByObjectType(HitDetails, Start, End, GetActorRotation().Quaternion(), ECC_WorldStatic, Collider->GetCollisionShape());

	if (!bIsHit)
		return false;

	Velocity += HitDetails.ImpactNormal.Cross(GetActorForwardVector()) * BuildingAvoidanceStrength * DeltaTime;
	return true;
}

void ABoid::LookForward()
{
	SetActorRotation(Velocity.Rotation().Add(-90,0,0));
}


void ABoid::OnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult)
{
	FVector Normal = HitResult.ImpactNormal;
	double VelocityMagnitude = Velocity.Length();
	FVector NormalizedVelocity = Velocity.GetClampedToSize(1,1);

	Velocity = NormalizedVelocity - 2 * Normal * Normal.Dot(NormalizedVelocity);
	AddActorWorldOffset(Normal);
	Velocity *= VelocityMagnitude;
}
