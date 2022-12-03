// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"

#include "Falcon.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Debug/ReporterGraph.h"

// Sets default values
ABoidManager::ABoidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
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
			FActorSpawnParameters Parameters;
			Parameters.Owner = this;
			const FRotator Rotator;
			FVector SpawnLocation;
			FVector Ranges = BoxDimensions / 2;
			
			for(int i = 0; i<BoidCount; i++)
			{
				SpawnLocation = Center
					+ FVector::RightVector * FMath::RandRange(-Ranges.X,Ranges.X)
					+ FVector::ForwardVector * FMath::RandRange(-Ranges.Y,Ranges.Y)
					+ FVector::UpVector * FMath::RandRange(-Ranges.Z,Ranges.Z);
				ABoid* Boid = World->SpawnActor<ABoid>(BoidBlueprint, SpawnLocation, Rotator, Parameters);
				Boid->Initialize(&Boids);
				Boids.Add(Boid);
			}
		}
		for (AFalcon* Falcon : Falcons)
		{
			Falcon->Initialize(&Boids);
		}
	}
}

