// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidHuntCharacter.h"

#include "BoidHuntGameState.h"
#include "BoidHuntHUD.h"
#include "BoidHuntUI.h"
#include "BoidManagerSubsystem.h"
#include "Building.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ToolBuilderUtil.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABoidHuntCharacter::ABoidHuntCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoidHuntCharacter::BeginPlay()
{
	Super::BeginPlay();
	JetpackFuel = MaxJetpackFuel;
	if (UWorld* World = GetWorld())
	{
		ABoidHuntHUD* HUD = Cast<ABoidHuntHUD>(World->GetFirstPlayerController()->GetHUD());
		if (HUD)
		{
			HUD->SetAmmo(Ammo);
		}
	}
}


// Called every frame
void ABoidHuntCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	JetpackFuel += DeltaTime * JetpackFuelGainPerSecond;

	if (JetpackFuel > MaxJetpackFuel)
	{
		JetpackFuel = MaxJetpackFuel;
	}

	if (UWorld* World = GetWorld())
	{
		ABoidHuntGameState* GameState = World->GetGameState<ABoidHuntGameState>();
		if (GameState)
		{
			GameState->SetFuel(JetpackFuel, MaxJetpackFuel);
		}
	}

	if (IsJetpackLockedOut && JetpackFuel > JetpackFuelUnlockThreshold)
	{
		IsJetpackLockedOut = false;
	}
}

void ABoidHuntCharacter::TryWallJump()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FHitResult Hit;

		FCollisionQueryParams Params;
		double HalfHeight = GetSimpleCollisionHalfHeight();

		World->SweepSingleByObjectType(
			Hit,
			GetActorLocation() + HalfHeight * FVector::UpVector,
			GetActorLocation() - GetActorForwardVector(),
			FRotator::ZeroRotator.Quaternion(),
			ECC_WorldStatic,
			FCollisionShape::MakeSphere(HalfHeight)
		);

		if (Cast<ABuilding>(Hit.GetActor()))
		{
			FVector Impulse = (Hit.ImpactNormal + FVector::UpVector) * WallJumpImpulse;
			GetCharacterMovement()->StopMovementImmediately();
			GetCharacterMovement()->AddImpulse(Impulse);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Impulse.ToString());
		}
	}
}

void ABoidHuntCharacter::OnJumpAction()
{
	if (!GetMovementComponent()->IsMovingOnGround())
	{
		TryWallJump();
	}
	else
	{
		Jump();
	}
}

void ABoidHuntCharacter::OnMoveAction(const FInputActionValue& InputActionValue)
{
	FVector2D Direction = InputActionValue.Get<FVector2D>();
	FVector Direction3D = FVector(Direction.X, Direction.Y, 0);
	Direction3D = GetActorRotation().RotateVector(Direction3D);
	AddMovementInput(Direction3D);
}

FVector ABoidHuntCharacter::GetFiringLocation() const
{
	return GetActorLocation() + BaseEyeHeight * FVector::UpVector;
}

void ABoidHuntCharacter::OnFireAction()
{
	if (Ammo < 1)
	{
		return;
	}
	Ammo--;

	UWorld* World = GetWorld();
	if (World && BoidManager == nullptr)
	{
		ABoidHuntGameState* GameState = static_cast<ABoidHuntGameState*>(World->GetGameState());
		if (GameState)
		{
			BoidManager = GameState->BoidManager;
		}
		ABoidHuntHUD* HUD = Cast<ABoidHuntHUD>(World->GetFirstPlayerController()->GetHUD());
		if (HUD)
		{
			HUD->SetAmmo(Ammo);
		}
	}
	if (World)
	{
		ABoidHuntHUD* HUD = Cast<ABoidHuntHUD>(World->GetFirstPlayerController()->GetHUD());
		if (HUD)
		{
			HUD->SetAmmo(Ammo);
		}
	}
	BoidManager->SpawnFalcon(GetFiringLocation(), GetBaseAimRotation().Vector());
}

void ABoidHuntCharacter::OnGlideAction()
{
	if (IsJetpackLockedOut)
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		// Spend 1 Fuel per second plus cancel fuel gains from this tick.		
		JetpackFuel -= World->GetDeltaSeconds() * (1 + JetpackFuelGainPerSecond);
		GetCharacterMovement()->AddImpulse(FVector::UpVector * JetpackForce);
		if (JetpackFuel < 0)
		{
			IsJetpackLockedOut = true;
		}
	}
}

void ABoidHuntCharacter::OnLookAction(const FInputActionValue& InputActionValue)
{
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

// Called to bind functionality to input
void ABoidHuntCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (JumpAction)
		{
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this,
			                                         &ABoidHuntCharacter::OnJumpAction);
		}
		if (GlideAction)
		{
			PlayerEnhancedInputComponent->BindAction(GlideAction, ETriggerEvent::Triggered, this,
			                                         &ABoidHuntCharacter::OnGlideAction);
		}
		if (MoveAction)
		{
			PlayerEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
			                                         &ABoidHuntCharacter::OnMoveAction);
		}
		if (FireAction)
		{
			PlayerEnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this,
			                                         &ABoidHuntCharacter::OnFireAction);
		}
		if (GlideAction)
		{
			PlayerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
			                                         &ABoidHuntCharacter::OnLookAction);
		}
	}
}

void ABoidHuntCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	// Make sure that we have a valid PlayerController.
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
			Subsystem->ClearAllMappings();

			// Add each mapping context, along with their priority values. Higher values outprioritize lower values.
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}
