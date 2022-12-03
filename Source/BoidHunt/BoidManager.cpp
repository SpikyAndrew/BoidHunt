// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"

#include "Falcon.h"
#include "Boid.h"

// Sets default values
ABoidManager::ABoidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
}

FBounds3d ABoidManager::GetBounds() const
{
	if (LevelBuilder)
		return LevelBuilder->GetBounds();

	FBounds3d Bounds = FBounds3d();
	Bounds.Max = FVector(MAX_dbl, MAX_dbl, MAX_dbl);
	Bounds.Min = FVector(MIN_dbl, MIN_dbl, MIN_dbl);
	return Bounds;
}

const TArray<const ABoid*>* ABoidManager::GetBoids() const
{
	return &Boids;
}

const TArray<AFalcon*>* ABoidManager::GetFalcons() const
{
	return &Falcons;
}

void ABoidManager::SpawnBoid(FVector Location)
{
	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters Parameters;
		ABoid* Boid = World->SpawnActor<ABoid>(BoidBlueprint, Location, FRotator::ZeroRotator, Parameters);
		Boid->Initialize(this);
		Boid->Initialize(this);
		Boids.Add(Boid);
	}
}

// Called when the game starts or when spawned
void ABoidManager::BeginPlay()
{
	Super::BeginPlay();
	SpawnBoids();
}

void ABoidManager::SpawnBoids()
{
	if (BoidBlueprint)
	{
		if (UWorld* World = GetWorld())
		{
			const FVector Center = this->GetTransform().GetLocation();
			FVector Ranges = BoxDimensions / 2;
			
			for(int i = 0; i<BoidCount; i++)
			{
				FVector SpawnLocation = Center
					+ FVector::RightVector * FMath::RandRange(-Ranges.X,Ranges.X)
					+ FVector::ForwardVector * FMath::RandRange(-Ranges.Y,Ranges.Y)
					+ FVector::UpVector * FMath::RandRange(-Ranges.Z,Ranges.Z);
				SpawnBoid(SpawnLocation);
			}
		}
		
		for (AFalcon* Falcon : Falcons)
		{
			Falcon->Initialize(this);
		}
	}
}

