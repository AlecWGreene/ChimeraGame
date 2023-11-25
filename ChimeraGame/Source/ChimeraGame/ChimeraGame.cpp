// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChimeraGame.h"
#include "Modules/ModuleManager.h"

class FChimeraGameModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
	}

	virtual void ShutdownModule() override
	{
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, ChimeraGame, "ChimeraGame" );
 