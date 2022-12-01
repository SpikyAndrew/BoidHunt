// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

UCLASS()
class BOIDHUNT_API ABoid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialize(const TArray<const ABoid*>* Boids);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void ApplySeparationRule(float DeltaTime);
	void ApplyAlignmentRule(float DeltaTime);
	void ApplyCohesionRule(float DeltaTime);
	void StayInBounds(float DeltaTime);
	void MoveWithVelocity(float DeltaTime);
	
	void LookForward();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	double SeparationStrength;
	UPROPERTY(EditAnywhere)
	double AlignmentStrength;
	UPROPERTY(EditAnywhere)
	double CohesionStrength;
	UPROPERTY(EditAnywhere)
	double BoundsStrength;
	UPROPERTY(EditAnywhere)
	double FlockingRadius;
	UPROPERTY(EditAnywhere)
	double SeparationRadius;
	UPROPERTY(EditAnywhere)
	double MaxVelocityDownwards;
	UPROPERTY(EditAnywhere)
	double MaxVelocityUpwards;
	UPROPERTY(EditAnywhere)
	FVector MinBounds;
	UPROPERTY(EditAnywhere)
	FVector MaxBounds;

	const TArray<const ABoid*>* BoidsArray;
	bool IsAlive;
	FVector Velocity;

};
