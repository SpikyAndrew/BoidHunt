// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyerBase.h"

// Sets default values
AFlyerBase::AFlyerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));

	RootComponent = Collider;
	Mesh->SetupAttachment(Collider);
	Mesh->SetRelativeLocation(FVector::ZeroVector);
}

// Called when the game starts or when spawned
void AFlyerBase::BeginPlay()
{
	Super::BeginPlay();
	IsAlive = true;
	Velocity = FMath::VRand() * MaxVelocityDownwards;
}

// Called every frame
void AFlyerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	IsAvoiding =  AvoidObstacles(DeltaTime);
	IsOutOfBounds = StayInBounds(DeltaTime);

	if (BoidsArray && !IsAvoiding && !IsOutOfBounds)
		SteerTowardsGoals(DeltaTime);
	
	double MaxVelocity = FMath::Lerp(MaxVelocityDownwards, MaxVelocityUpwards,
		(1 + Velocity.GetClampedToSize(1,1).Dot(FVector::UpVector))/2);
	Velocity = Velocity.GetClampedToSize(MinVelocity,MaxVelocity);
	MoveWithVelocity(DeltaTime);
	LookForward();
}

// Sets BoidsArray if it hasn't been set to anything yet.
void AFlyerBase::Initialize(const TArray<const ABoid*>* Boids)
{
	if (!BoidsArray)
		BoidsArray = Boids;
}

bool AFlyerBase::GetIsAlive() const
{
	return IsAlive;
}

bool AFlyerBase::StayInBounds(float DeltaTime)
{
	const FVector Location = GetActorLocation();

	IsOutOfBounds = false;
	
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

void AFlyerBase::MoveWithVelocity(float DeltaTime)
{
	AddActorWorldOffset(Velocity * DeltaTime, true);
}

bool AFlyerBase::AvoidObstacles(float DeltaTime)
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

void AFlyerBase::LookForward()
{
	SetActorRotation(Velocity.Rotation().Add(-90,0,0));
}

void AFlyerBase::SteerTowardsGoals(float DeltaTime)
{
	
}

void AFlyerBase::BounceOnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                       UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult)
{
	FVector Normal = HitResult.ImpactNormal;
	double VelocityMagnitude = Velocity.Length();
	FVector NormalizedVelocity = Velocity.GetClampedToSize(1,1);

	Velocity = NormalizedVelocity - 2 * Normal * Normal.Dot(NormalizedVelocity);
	AddActorWorldOffset(Normal);
	Velocity *= VelocityMagnitude;
}
