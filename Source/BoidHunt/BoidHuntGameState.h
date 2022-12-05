// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boid.h"
#include "GameFramework/GameStateBase.h"
#include "BoidHuntGameState.generated.h"

/**
 * 
 */
UCLASS()
class BOIDHUNT_API ABoidHuntGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	void AddBoids(int Number);
	void AddFalcons(int Number);
	int GetNumberOfBoids();
	int GetNumberOfFalcons();

	UPROPERTY()
	ABoidManager* BoidManager;

protected:
	int NumberOfBoids;
	int NumberOfFalcons;
};
