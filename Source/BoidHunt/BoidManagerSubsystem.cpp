// // Fill out your copyright notice in the Description page of Project Settings.
//
//
// #include "BoidManagerSubsystem.h"
//
// #include "Boid.h"
// #include "BoidManager.h"
// #include "Falcon.h"
// #include "LevelBuilder.h"
//
// void UBoidManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
// {
// 	Super::Initialize(Collection);
// 	SpawnBoids();
// }
//
// FBounds3d UBoidManagerSubsystem::GetBounds() const
// {
// 	if (LevelBuilder)
// 		return LevelBuilder->GetBounds();
//
// 	FBounds3d Bounds = FBounds3d();
// 	Bounds.Max = FVector(MAX_dbl, MAX_dbl, MAX_dbl);
// 	Bounds.Min = FVector(MIN_dbl, MIN_dbl, MIN_dbl);
// 	return Bounds;
// }
//
// const TArray<const ABoid*>* UBoidManagerSubsystem::GetBoids() const
// {
// 	return &Boids;
// }
//
// const TArray<AFalcon*>* UBoidManagerSubsystem::GetFalcons() const
// {
// 	return &Falcons;
// }
//
// void UBoidManagerSubsystem::SpawnBoid(FVector Location)
// {
// 	if (UWorld* World = GetWorld())
// 	{
// 		FActorSpawnParameters Parameters;
// 		ABoid* Boid = World->SpawnActor<ABoid>(BoidBlueprint, Location, FRotator::ZeroRotator, Parameters);
// 		Boid->Initialize(this);
// 		Boids.Add(Boid);
// 	}
// }
//
// void UBoidManagerSubsystem::SpawnFalcon(FVector Location, FVector Velocity)
// {
// 	if (UWorld* World = GetWorld())
// 	{
// 		FActorSpawnParameters Parameters;
// 		AFalcon* Falcon = World->SpawnActor<AFalcon>(FalconBlueprint, Location, FRotator::ZeroRotator, Parameters);
// 		Falcon->Initialize(this, Velocity);
// 		Falcons.Add(Falcon);
// 	}
// }
//
// void UBoidManagerSubsystem::SpawnBoids()
// {
// 	if (BoidBlueprint)
// 	{
// 		if (UWorld* World = GetWorld())
// 		{
// 			FVector Ranges = BoxDimensions / 2;
// 			
// 			for(int i = 0; i<BoidCount; i++)
// 			{
// 				FVector SpawnLocation = Center
// 					+ FVector::RightVector * FMath::RandRange(-Ranges.X,Ranges.X)
// 					+ FVector::ForwardVector * FMath::RandRange(-Ranges.Y,Ranges.Y)
// 					+ FVector::UpVector * FMath::RandRange(-Ranges.Z,Ranges.Z);
// 				SpawnBoid(SpawnLocation);
// 			}
// 		}
// 	}
// }
//
