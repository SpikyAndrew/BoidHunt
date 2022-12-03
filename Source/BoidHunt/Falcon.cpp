// Fill out your copyright notice in the Description page of Project Settings.


#include "Falcon.h"

#include "Boid.h"

#include "Math/NumericLimits.h"
#include "DrawDebugHelpers.h"

AFalcon::AFalcon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AFalcon::OnHitCheckForBoid(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector Vector, const FHitResult& HitResult)
{
	ABoid* BoidHit = Cast<ABoid>(OtherActor);
	if(BoidHit)
		BoidHit->Deactivate();
	else
		BounceOnHit(PrimitiveComponent, OtherActor, OtherComponent, Vector, HitResult);		
}

void AFalcon::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentHit.AddUniqueDynamic(this, &AFalcon::OnHitCheckForBoid);
}


void AFalcon::Tick(float DeltaSeconds)
{
	PreviousVelocity = Velocity;
	Super::Tick(DeltaSeconds);
	Energy -= (Velocity - PreviousVelocity).Length();
}

void AFalcon::SteerTowardsGoals(float DeltaTime)
{
	Super::SteerTowardsGoals(DeltaTime);

	FVector RelativePositionOfPrey = GetRelativePositionOfPrey();
	RelativePositionOfPrey.Normalize();
	Velocity += RelativePositionOfPrey * SteeringStrength * DeltaTime;
}

FVector AFalcon::GetRelativePositionOfPrey() const
{
	FVector Location = GetActorLocation();
	double MinSquaredDistance = MAX_dbl;
	// In case we never find a target, fly forward.
	FVector RelativePositionOfPrey = GetActorForwardVector();
	
	for (const ABoid* Boid : *BoidsArray)
	{
		if (!Boid->GetIsAlive())
			continue;
		
		FVector RelativePosition = Boid->GetActorLocation() - Location;
		const double SquaredDistance = RelativePosition.SquaredLength();
		if (SquaredDistance < MinSquaredDistance)
		{
			MinSquaredDistance = SquaredDistance;
			RelativePositionOfPrey = RelativePosition;
		}
	}
	DrawDebugLine(GetWorld(), Location, Location + RelativePositionOfPrey, FColor::Green, false, -1, 0, 5);
	return RelativePositionOfPrey;
}