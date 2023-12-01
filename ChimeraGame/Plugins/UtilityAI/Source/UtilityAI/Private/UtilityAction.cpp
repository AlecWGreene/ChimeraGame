// Copyright Alec Greene. All Rights Reserved.
#include "UtilityAction.h"

UUtilityAction::UUtilityAction()
{
}

float UUtilityAction::ComputeDesire_Implementation(float DeltaTime, float PreviousDesire, const FUtilityFactors& Factors) const
{
	return 0.0f;
}

bool UUtilityAction::CanActivate_Implementation(const FUtilityFactors& Factors) const
{
	return false;
}

void UUtilityAction::Activate_Implementation(const FUtilityFactors& Factors)
{
}

void UUtilityAction::Tick_Implementation(float DeltaTime)
{
}

void UUtilityAction::End_Implementation()
{
}
