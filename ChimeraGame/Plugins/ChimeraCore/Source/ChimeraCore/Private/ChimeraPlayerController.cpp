// Copyright Alec Greene. All Rights Reserved.

#include "ChimeraPlayerController.h"
#include "ChimeraGAS/Public/ChimeraAbilitySystemComponent.h"
#include "ChimeraInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

DEFINE_LOG_CATEGORY_STATIC(LogChimeraPlayerController, Log, All);

void AChimeraPlayerController::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	// agreene 2023/12/4 - #ToDo #Input Figure out why we are binding too much
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->ClearActionEventBindings();
		EnhancedInputComponent->ClearActionValueBindings();
	}
}
