// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoidManager.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "BoidHuntCharacter.generated.h"

UCLASS()
class BOIDHUNT_API ABoidHuntCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoidHuntCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Does a WallJump if near a wall.
	void TryWallJump();
	void OnJumpAction();
	void OnMoveAction(const FInputActionValue& InputActionValue);
	void OnFireAction();
	// Adds vertical velocity using the jetpack.
	void OnGlideAction();
	void OnLookAction(const FInputActionValue& InputActionValue);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PawnClientRestart() override;
	// Returns the location from which Falcons are spawned.
	FVector GetFiringLocation() const;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere)
	UInputAction* FireAction;
	UPROPERTY(EditAnywhere)
	UInputAction* GlideAction;
	UPROPERTY(EditAnywhere)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere)
	double WallJumpImpulse;
	UPROPERTY(EditAnywhere)
	double MaxJetpackFuel;
	UPROPERTY(EditAnywhere)
	double JetpackFuelGainPerSecond;
	UPROPERTY(EditAnywhere)
	double JetpackFuelUnlockThreshold;
	UPROPERTY(EditAnywhere)
	double JetpackForce;
	UPROPERTY(EditAnywhere)
	int Ammo;
	UPROPERTY(EditAnywhere) // ReSharper disable once UnrealHeaderToolError
	UInputMappingContext* InputMappingContext;
	UPROPERTY()
	ABoidManager* BoidManager = nullptr;

	// Amount of seconds for which the player could use the jetpack.
	double JetpackFuel;
	// Set to true when out of Fuel, then false once Fuel reaches JetpackFuelUnlockThreshold.
	bool IsJetpackLockedOut;
};
