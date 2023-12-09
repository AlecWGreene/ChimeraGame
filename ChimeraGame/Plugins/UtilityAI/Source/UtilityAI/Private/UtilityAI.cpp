// Copyright Epic Games, Inc. All Rights Reserved.

#include "UtilityAI.h"

#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"

#include "UtilityAIComponent.h"

#define LOCTEXT_NAMESPACE "FUtilityAIModule"

void FUtilityAIModule::StartupModule()
{
	AHUD::OnShowDebugInfo.AddStatic(&UUtilityAIComponent::OnDisplayDebug);
}

void FUtilityAIModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUtilityAIModule, UtilityAI)