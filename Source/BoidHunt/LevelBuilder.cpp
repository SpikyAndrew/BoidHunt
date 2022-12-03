// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelBuilder.h"

#include "GameFramework/GameStateBase.h"

// Sets default values
ALevelBuilder::ALevelBuilder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
}

FBounds3d ALevelBuilder::CalculateLevelBounds() const
{
	FBounds3d CalculatedBounds;
	CalculatedBounds.Min = FVector(-Width/2 * CellSize, -Length/2 * CellSize,500);
	CalculatedBounds.Max = FVector(Width/2 * CellSize,Length/2 * CellSize,6000);
	return CalculatedBounds;
}

// Called when the game starts or when spawned
void ALevelBuilder::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector(CellSize/2, CellSize/2, 0));
	Bounds = CalculateLevelBounds();
	Spawn();
}

void ALevelBuilder::Spawn()
{
	if (ActorToSpawn)
	{
		if (UWorld* World = GetWorld())
		{
			const FVector Center = this->GetTransform().GetLocation();
			FActorSpawnParameters Parameters;
			Parameters.Owner = this;
			FVector SpawnLocation;
			for(int x = 0; x<Width; x++)
			{
				for (int y = 0; y<Length; y++)
				{
					SpawnLocation = Center
						+ FVector::RightVector * (x - Width/2) * CellSize
						+ FVector::ForwardVector * (y - Length/2) * CellSize;
					AActor* Actor = World->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, FRotator::ZeroRotator, Parameters);

					Actor->SetActorScale3D(FVector(BuildingWidth,BuildingWidth, BuildingHeights[x + y * Width]));
				}
			}
		}
	}
}

// Called every frame
void ALevelBuilder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FBounds3d ALevelBuilder::GetBounds() const
{
	return Bounds;
}

