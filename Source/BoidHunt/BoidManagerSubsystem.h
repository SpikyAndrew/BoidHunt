// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "Math/Bounds.h"
// #include "Subsystems/WorldSubsystem.h"
// #include "BoidManagerSubsystem.generated.h"
//
// class AFalcon;
// class ABoid;
// class ALevelBuilder;
// /**
//  * 
//  */
// UCLASS()
// class BOIDHUNT_API UBoidManagerSubsystem : public UWorldSubsystem
// {
// 	GENERATED_BODY()
// public:	
// 	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
// 	FBounds3d GetBounds() const;
// 	const TArray<const ABoid*>* GetBoids() const;
// 	const TArray<AFalcon*>* UBoidManagerSubsystem::GetFalcons() const;
// 	void SpawnBoid(FVector Location);
// 	void SpawnFalcon(FVector Location, FVector Velocity);
// 	void SpawnBoids();
//
// protected:
// 	UPROPERTY(EditAnywhere)
// 	TSubclassOf<ABoid> BoidBlueprint;
// 	UPROPERTY(EditAnywhere)
// 	TSubclassOf<ABoid> FalconBlueprint;
// 	UPROPERTY(EditAnywhere)
// 	int BoidCount;
// 	UPROPERTY()
// 	TArray<const ABoid*> Boids;
// 	UPROPERTY()
// 	TArray<AFalcon*> Falcons;
// 	UPROPERTY(EditInstanceOnly)
// 	ALevelBuilder* LevelBuilder;
// 	// Dimensions of the box that boids will spawn in
// 	UPROPERTY(EditAnywhere)
// 	FVector BoxDimensions;
// 	UPROPERTY(EditAnywhere)
// 	FVector Center;
// };
