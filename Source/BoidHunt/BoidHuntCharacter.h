// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoidManager.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "BoidHuntCharacter.generated.h"

UCLASS()
class BOIDHUNT_API ABoidHuntCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoidHuntCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void TryWallJump();
	void OnJumpAction();
	void OnMoveAction(const FInputActionValue& InputActionValue);
	FVector GetFiringLocation() const;
	void OnFireAction();
	void OnGlideAction();
	void OnLookAction(const FInputActionValue& InputActionValue);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PawnClientRestart() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
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
	UPROPERTY(EditAnywhere)
	// ReSharper disable once UnrealHeaderToolError
	UInputMappingContext* InputMappingContext;
	
	ABoidManager* BoidManager = nullptr;
	
	double JetpackFuel;
	bool IsJetpackLockedOut;

};
