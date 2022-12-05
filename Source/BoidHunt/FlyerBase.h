// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoidManager.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NiagaraSystem.h"
#include "FlyerBase.generated.h"

class ABoid;
UCLASS()
class BOIDHUNT_API AFlyerBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFlyerBase();

	// Bounces the Flyer away based on the impact normal.
	UFUNCTION()
	void BounceOnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1,
	                 FVector Vector, const FHitResult& HitResult);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called when the Flyer dies/transforms.
	virtual void Deactivate();
	void Initialize(ABoidManager* BoidManager);
	// Returns false if the Flyer has died/transformed.
	bool GetIsAlive() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Child class specific steering behaviour, performed when the flyer is in Bounds and not avoiding an obstacle.
	virtual void SteerTowardsGoals(float DeltaTime);
	// Steers towards the play area.
	bool StayInBounds(float DeltaTime);
	// Sweeps for obstacles and steers sideways if there is one.
	bool AvoidObstacles(float DeltaTime);
	// Moves the Flyer.
	void MoveWithVelocity(float DeltaTime);
	// Rotates the Flyer to face where its heading.
	void LookForward();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	UBoxComponent* Collider;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ParticleSystem;
	
	UPROPERTY(EditAnywhere)
	double MinVelocity;
	UPROPERTY(EditAnywhere)
	double MaxVelocityDownwards;
	UPROPERTY(EditAnywhere)
	double MaxVelocityUpwards;
	UPROPERTY(EditAnywhere)
	double BoundsStrength;
	UPROPERTY(EditAnywhere)
	double BuildingAvoidanceStrength;
	// How many seconds before collision should the Flyer start avoiding it.
	UPROPERTY(EditAnywhere)
	double BuildingAvoidanceEagerness;
	UPROPERTY()
	ABoidManager* BoidManager;
	bool IsAlive;
	FVector Velocity;
	bool IsAvoiding;
	bool IsOutOfBounds;

	// Directions of possible adjacent partitions.
	TArray<FIntVector2> Directions =
    	{
    		FIntVector2(0,0),
    		FIntVector2(1,0),
    		FIntVector2(-1,0),
    		FIntVector2(0,1),
    		FIntVector2(1,1),
    		FIntVector2(-1,1),
    		FIntVector2(0,-1),
    		FIntVector2(1,-1),
    		FIntVector2(-1,-1),
    	};
};
