// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChimeraGameCharacter.h"

#include "Camera/CameraComponent.h"
#include "ChimeraInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "ChimeraGASFunctionLibrary.h"
#include "ChimeraAbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "InputMappingContext.h"

//////////////////////////////////////////////////////////////////////////
// Constructor and Engine Events
//////////////////////////////////////////////////////////////////////////

AChimeraGameCharacter::AChimeraGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Interface Overrides
//////////////////////////////////////////////////////////////////////////

void AChimeraGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	UChimeraInputComponent* ChimeraInputComp = Cast<UChimeraInputComponent>(PlayerInputComponent);
	check(ChimeraInputComp);

	APlayerController* PC = GetController<APlayerController>();
	check(PC);

	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->Player);
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* LPSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(LPSubsystem);

	if (DefaultPlayerInputMappingContext.IsValid())
	{
		LPSubsystem->AddMappingContext(DefaultPlayerInputMappingContext.LoadSynchronous(), 0);
	}

	if (DefaultPlayerInputConfig.IsValid())
	{
		const UChimeraInputConfig* DefaultInputConfig = DefaultPlayerInputConfig.LoadSynchronous();
		check(DefaultInputConfig);

		if (UChimeraAbilitySystemComponent* ASC = UChimeraGASFunctionLibrary::GetChimeraASC(this))
		{
			ChimeraInputComp->BindAbilityActions(DefaultInputConfig, ASC, &UChimeraAbilitySystemComponent::AbilityInput_Pressed, &UChimeraAbilitySystemComponent::AbilityInput_Released);
		}

		if (InputMoveTag.IsValid())
		{
			ChimeraInputComp->BindNativeAction(DefaultInputConfig, InputLookTag, ETriggerEvent::Triggered, this, &AChimeraGameCharacter::HandleMoveInput);
		}

		if (InputLookTag.IsValid())
		{
			ChimeraInputComp->BindNativeAction(DefaultInputConfig, InputLookTag, ETriggerEvent::Triggered, this, &AChimeraGameCharacter::HandleLookInput);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Player Input
//////////////////////////////////////////////////////////////////////////

void AChimeraGameCharacter::HandleMoveInput(const FInputActionValue& InputActionValue)
{
	AController* Controller = GetController<AController>();
	if (!Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot move with invalid controller."));
		return;
	}

	const FVector2D InputValue = InputActionValue.Get<FVector2D>();
	const FRotator YawRotation = FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f); 

	const FVector ForwardDirection = Input.Y * YawRotation.RotateVector(FVector::ForwardVector);
	const FVector RightDirection = Input.X * YawRotation.RotateVector(FVector::RightVector);

	AddMovementInput(ForwardDirection + RightDirection, 1.f);
}

void AChimeraGameCharacter::HandleLookInput(const FInputActionValue& InputActionValue)
{
	AController* Controller = GetController<AController>();
	if (!Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot look with invalid controller."));
		return;
	}

	// @agreene #Note #GamepadInput - 2023/06/24 - If adding gamepad support, this needs to get scaled by UWorld::GetDeltaSeconds
	const FVector2D InputValue = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(InputValue.Y);
}