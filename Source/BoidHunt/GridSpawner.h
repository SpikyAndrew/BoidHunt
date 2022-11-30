// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridSpawner.generated.h"

UCLASS()
class BOIDHUNT_API AGridSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridSpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Spawn();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere)
	int Width;
	UPROPERTY(EditAnywhere)
	int Length;
	UPROPERTY(EditAnywhere)
	double CellSize;
};
