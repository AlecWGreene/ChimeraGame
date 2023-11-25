// Copyright Alec Greene. All Rights Reserved.

#include "ChimeraPlayerController.h"
#include "ChimeraGAS/Public/ChimeraAbilitySystemComponent.h"
#include "ChimeraInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

DEFINE_LOG_CATEGORY_STATIC(LogChimeraPlayerController, Log, All);

void AChimeraPlayerController::PostProcessInput(const float DeltaTime, const bool bPaused)
{
	UChimeraAbilitySystemComponent* ASC = nullptr;
	if (ASC)
	{
		ASC->ProcessAbilityInput(DeltaTime, bPaused);
	}

	Super::PostProcessInput(DeltaTime, bPaused);
}