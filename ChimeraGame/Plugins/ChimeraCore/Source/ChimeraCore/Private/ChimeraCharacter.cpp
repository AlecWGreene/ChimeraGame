// Copyright Alec Greene. All Rights Reserved.

#include "ChimeraCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayTagContainer.h"
#include "InputMappingContext.h"

#include "ChimeraCoreTags.h"
#include "ChimeraGASFunctionLibrary.h"
#include "ChimeraInputComponent.h"
#include "ChimeraPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogChimeraCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////////

AChimeraCharacter::AChimeraCharacter()
{
	// agreene 2023/11/25 - #Todo #Character all this should be tunable

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
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

	// Setup AbilitySystemComponent
	AbilitySystemComponent = CreateDefaultSubobject<UChimeraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AChimeraCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UChimeraInputComponent* ChimeraInputComp = Cast<UChimeraInputComponent>(PlayerInputComponent);
	check(ChimeraInputComp);

	AChimeraPlayerController* PC = GetController<AChimeraPlayerController>();
	check(PC);

	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->Player);
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* LPSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(LPSubsystem);

	for (const UInputMappingContext* DefaultIMC : PC->DefaultInputMappingContexts)
	{
		if (!ensureAlwaysMsgf(DefaultIMC, TEXT("Invalid default InputMappingContext.")))
		{
			continue;
		}

		LPSubsystem->AddMappingContext(DefaultIMC, 0);
	}

	if (PC->DefaultInputMappingConfig)
	{
		ChimeraInputComp->BindAbilityActions(PC->DefaultInputMappingConfig, AbilitySystemComponent.Get(), &UChimeraAbilitySystemComponent::AbilityInput_Pressed, &UChimeraAbilitySystemComponent::AbilityInput_Released);

		ChimeraInputComp->BindNativeAction(PC->DefaultInputMappingConfig, ChimeraCoreTags::InputTag_Move, ETriggerEvent::Triggered, this, &AChimeraCharacter::HandleMoveInput);
		ChimeraInputComp->BindNativeAction(PC->DefaultInputMappingConfig, ChimeraCoreTags::InputTag_Look, ETriggerEvent::Triggered, this, &AChimeraCharacter::HandleLookInput);
	}
	else
	{
		UE_LOG(LogChimeraCharacter, Error, TEXT("Invalid default InputMappingConfig."));
	}
}

//////////////////////////////////////////////////////////////////////////
// Player Input
//////////////////////////////////////////////////////////////////////////

void AChimeraCharacter::HandleMoveInput(const FInputActionValue& InputActionValue)
{
	AController* CharacterController = GetController<AController>();
	if (!CharacterController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot move with invalid controller."));
		return;
	}

	const FVector2D InputValue = InputActionValue.Get<FVector2D>();
	const FRotator YawRotation = FRotator(0.f, CharacterController->GetControlRotation().Yaw, 0.f);

	const FVector ForwardDirection = InputValue.Y * YawRotation.RotateVector(FVector::ForwardVector);
	const FVector RightDirection = InputValue.X * YawRotation.RotateVector(FVector::RightVector);

	AddMovementInput(ForwardDirection + RightDirection, 1.f);
}

void AChimeraCharacter::HandleLookInput(const FInputActionValue& InputActionValue)
{
	// @agreene 2023/06/24 -- #Note #GamepadInput If adding gamepad support, this needs to get scaled by UWorld::GetDeltaSeconds
	const FVector2D InputValue = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(InputValue.Y);
}