// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

UCLASS()
class BOIDHUNT_API ABoid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();
	UFUNCTION()
	void OnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialize(const TArray<const ABoid*>* Boids);
	
protected:
		// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void ApplySeparationRule(float DeltaTime);
	void ApplyAlignmentRule(float DeltaTime);
	void ApplyCohesionRule(float DeltaTime);
	bool StayInBounds(float DeltaTime);
	bool AvoidObstacles(float DeltaTime);
	void MoveWithVelocity(float DeltaTime);
	void LookForward();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	UBoxComponent* Collider;
	UPROPERTY(EditAnywhere)
	double SeparationStrength;
	UPROPERTY(EditAnywhere)
	double AlignmentStrength;
	UPROPERTY(EditAnywhere)
	double CohesionStrength;
	UPROPERTY(EditAnywhere)
	double BoundsStrength;
	UPROPERTY(EditAnywhere)
	double BuildingAvoidanceStrength;
	UPROPERTY(EditAnywhere)
	double FlockingRadius;
	UPROPERTY(EditAnywhere)
	double BuildingAvoidanceEagerness;
	UPROPERTY(EditAnywhere)
	double SeparationRadius;
	UPROPERTY(EditAnywhere)
	double MinVelocity;
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
