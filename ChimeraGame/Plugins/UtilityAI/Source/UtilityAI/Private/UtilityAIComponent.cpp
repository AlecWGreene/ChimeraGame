// Copyright Alec Greene. All Rights Reserved.
#include "UtilityAIComponent.h"

#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"

#include "UtilityAction.h"

UUtilityAIComponent::UUtilityAIComponent()
{
}

void UUtilityAIComponent::InitializeComponent()
{
}

bool UUtilityAIComponent::IsRunning() const
{
	return false;
}

bool UUtilityAIComponent::IsPaused() const
{
	return false;
}

void UUtilityAIComponent::StartLogic()
{
}

void UUtilityAIComponent::PauseLogic(const FString& Reason)
{
}

void UUtilityAIComponent::RestartLogic()
{
}

void UUtilityAIComponent::StopLogic(const FString& Reason)
{
}

void UUtilityAIComponent::Cleanup()
{
}

FString UUtilityAIComponent::GetDebugInfoString() const
{
	FString Output;
	
	// Display Actions
	for (TPair<FGameplayTag, float> DesireData : ActionDesires)
	{
		Output += FString::Printf(TEXT("\t%s = %f\n"), *DesireData.Key.ToString(), DesireData.Value);
	}

	return Output;
}

void UUtilityAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateDesires(DeltaTime);
}

void UUtilityAIComponent::UpdateDesires(float DeltaTime)
{
	for (const TPair<FGameplayTag, UUtilityAction*>& ActionSlot : Actions)
	{
		if (IsValid(ActionSlot.Value))
		{
			float& Desire = ActionDesires.FindOrAdd(ActionSlot.Key);
			Desire = ActionSlot.Value->ComputeDesire(DeltaTime, Desire, GetBlackboardComponent());
		}
	}
}