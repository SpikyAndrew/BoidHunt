// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidHuntCharacter.h"

#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABoidHuntCharacter::ABoidHuntCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->FieldOfView = 90.f;
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector( 0,0,189.f));
}

// Called when the game starts or when spawned
void ABoidHuntCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoidHuntCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoidHuntCharacter::OnJumpAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Jumpy jump"));
	Jump();
}

void ABoidHuntCharacter::OnMoveAction(const FInputActionValue& InputActionValue)
{
	FVector2D Direction = InputActionValue.Get<FVector2D>();
	FVector Direction3D = FVector(Direction.X, Direction.Y, 0);
	Direction3D = GetActorRotation().RotateVector(Direction3D);
	AddMovementInput(Direction3D);
}

void ABoidHuntCharacter::OnFireAction()
{
}

void ABoidHuntCharacter::OnGlideAction()
{
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
	UE_LOG(LogTemp, Warning, TEXT("Trying to bind"));

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // There are ways to bind a UInputAction* to a handler function and multiple types of ETriggerEvent that may be of interest.
		UE_LOG(LogTemp, Warning, TEXT("Cast successful"));

        if (JumpAction)
            PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABoidHuntCharacter::OnJumpAction);
		if (MoveAction)
			PlayerEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABoidHuntCharacter::OnMoveAction);
		if (FireAction)
			PlayerEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABoidHuntCharacter::OnFireAction);
		if (GlideAction)
			PlayerEnhancedInputComponent->BindAction(GlideAction, ETriggerEvent::Triggered, this, &ABoidHuntCharacter::OnGlideAction);
		if (GlideAction)
			PlayerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABoidHuntCharacter::OnLookAction);

		// // This calls the handler function (a UFUNCTION) by name on every tick while the input conditions are met, such as when holding a movement key down.
        // if (MyOtherInputAction)
        // {
        //     PlayerEnhancedInputComponent->BindAction(MyOtherInputAction, ETriggerEvent::Triggered, this, TEXT("MyOtherInputHandlerFunction"));
        // }
    }
}

void ABoidHuntCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	// Make sure that we have a valid PlayerController.
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
			Subsystem->ClearAllMappings();

			// Add each mapping context, along with their priority values. Higher values outprioritize lower values.
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}
