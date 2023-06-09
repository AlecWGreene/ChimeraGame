// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChimeraGameGameMode.h"
#include "ChimeraGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AChimeraGameGameMode::AChimeraGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
