// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Bounds.h"
#include "LevelBuilder.generated.h"

UCLASS(Config=Game)
class BOIDHUNT_API ALevelBuilder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelBuilder();
	FBounds3d CalculateLevelBounds() const;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FBounds3d GetBounds() const;

protected:
	void ValidateAndFixConfig();
	static void PrintValidationWarning(FString Text);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Spawn();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	double CellSize;
	UPROPERTY(EditAnywhere)
	double BuildingWidth;
	
	UPROPERTY(Config)
	int Width;
	UPROPERTY(Config)
	int Length;
	UPROPERTY(Config)
	TArray<double> BuildingHeights;

	FBounds3d Bounds;
	
	const int MinLevelSize = 1;
};
