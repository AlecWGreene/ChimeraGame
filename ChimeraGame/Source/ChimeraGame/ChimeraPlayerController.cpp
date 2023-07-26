#include "ChimeraPlayerController.h"
#include "ChimeraGAS/Public/ChimeraAbilitySystemComponent.h"

void AChimeraPlayerController::PostProcessInput(const float DeltaTime, const bool bPaused)
{
	UChimeraAbilitySystemComponent* ASC = nullptr;
	if (ASC)
	{
		ASC->ProcessAbilityInput(DeltaTime, bPaused);
	}

	Super::PostProcessInput(DeltaTime, bPaused);
}