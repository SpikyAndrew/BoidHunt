// Fill out your copyright notice in the Description page of Project Settings.


#include "Falcon.h"

#include "Boid.h"
#include "BoidHuntGameState.h"

#include "Math/NumericLimits.h"

AFalcon::AFalcon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFalcon::OnHitCheckForBoid(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComponent, FVector Vector, const FHitResult& HitResult)
{
	ABoid* BoidHit = Cast<ABoid>(OtherActor);
	if (BoidHit)
	{
		BoidHit->Deactivate();
		Energy += EnergyGainPerKill;
		// When we've hit a boid, start hunting normally.
		NoSteeringTimer = 0;
	}
	else
	{
		BounceOnHit(PrimitiveComponent, OtherActor, OtherComponent, Vector, HitResult);
	}
}

void AFalcon::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentHit.AddUniqueDynamic(this, &AFalcon::OnHitCheckForBoid);
	Energy = StartingEnergy;
}

void AFalcon::Deactivate()
{
	Super::Deactivate();
	if (UWorld* World = GetWorld())
	{
		ABoidHuntGameState* GameState = World->GetGameState<ABoidHuntGameState>();
		if (GameState)
		{
			GameState->AddFalcons(-1);
		}
	}
}

void AFalcon::Initialize(ABoidManager* Manager, const FVector& Direction)
{
	if (BoidManager)
	{
		return;
	}

	BoidManager = Manager;
	Velocity = Direction * MaxVelocityDownwards;
}

void AFalcon::Tick(float DeltaSeconds)
{
	NoSteeringTimer -= DeltaSeconds;

	PreviousVelocity = Velocity;

	if (NoSteeringTimer > 0)
	{
		MoveWithVelocity(DeltaSeconds);
		LookForward();
		return;
	}

	Super::Tick(DeltaSeconds);

	Energy -= (Velocity - PreviousVelocity).Length();
	if (Energy < 0)
	{
		BoidManager->SpawnBoid(GetActorLocation());
		Deactivate();
	}
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
	FIntVector2 PartitionKey = BoidManager->GetPartitionKeyFromLocation(GetActorLocation());
    TMultiMap<FIntVector2, ABoid*> BoidMap = *BoidManager->GetBoidMap();
	double MinSquaredDistance = MAX_dbl;
	// In case we never find a target, fly forward.
	FVector RelativePositionOfPrey = GetActorForwardVector();
	TArray<ABoid*>* BoidArray = BoidManager->GetBoidArray();
	
	for (const ABoid* Boid : *BoidArray)
	{
		if (!Boid->GetIsAlive())
		{
			continue;
		}

		FVector RelativePosition = Boid->GetActorLocation() - Location;
		const double SquaredDistance = RelativePosition.SquaredLength();
		if (SquaredDistance < MinSquaredDistance)
		{
			MinSquaredDistance = SquaredDistance;
			RelativePositionOfPrey = RelativePosition;
		}
	}

	return RelativePositionOfPrey;
}
