// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoidManager.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "FlyerBase.generated.h"

class ABoid;
UCLASS()
class BOIDHUNT_API AFlyerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlyerBase();
	
	UFUNCTION()
	void BounceOnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Deactivate();
	void Initialize(ABoidManager* BoidManager);
	bool GetIsAlive() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual	void SteerTowardsGoals(float DeltaTime);
	bool StayInBounds(float DeltaTime);
	bool AvoidObstacles(float DeltaTime);
	void MoveWithVelocity(float DeltaTime);
	void LookForward();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	UBoxComponent* Collider;
	
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
	UPROPERTY(EditAnywhere)
	double BuildingAvoidanceEagerness;
	UPROPERTY()
	ABoidManager* BoidManager;
	bool IsAlive;
	FVector Velocity;
	bool IsAvoiding;
	bool IsOutOfBounds;
};
