// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSpawner.h"

#include "GameFramework/GameStateBase.h"

// Sets default values
AGridSpawner::AGridSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
}

// Called when the game starts or when spawned
void AGridSpawner::BeginPlay()
{
	Super::BeginPlay();
	Spawn();
}

void AGridSpawner::Spawn()
{
	if (ActorToSpawn)
	{
		if (UWorld* World = GetWorld())
		{
			const FVector Center = this->GetTransform().GetLocation();
			FActorSpawnParameters Parameters;
			Parameters.Owner = this;
			const FRotator Rotator;
			FVector SpawnLocation;
			for(int x = -Width/2; x<Width/2; x++)
			{
				for (int y = -Length/2; y<Length/2; y++)
				{
					SpawnLocation = Center
						+ FVector::RightVector * x * CellSize
						+ FVector::ForwardVector * y * CellSize;
					World->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, Rotator, Parameters);
				}
			}
		}
	}
}

// Called every frame
void AGridSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

