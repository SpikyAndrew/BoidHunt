// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"

#include "Debug/ReporterGraph.h"

// Sets default values
ABoidManager::ABoidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

TArray<const ABoid*>* ABoidManager::GetBoids()
{
	return &boids;
}

// Called when the game starts or when spawned
void ABoidManager::BeginPlay()
{
	Super::BeginPlay();
}

