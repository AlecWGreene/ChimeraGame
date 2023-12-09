// Copyright Alec Greene. All Rights Reserved.
#include "UtilityAction.h"

#include "BehaviorTree/BlackboardComponent.h"

UUtilityAction::UUtilityAction()
{
}

float UUtilityAction::ComputeDesire_Implementation(float DeltaTime, float PreviousDesire, const class UBlackboardComponent* BBComponent) const
{
	return 0.0f;
}

bool UUtilityAction::CanActivate_Implementation(const class UBlackboardComponent* BBComponent) const
{
	return false;
}

void UUtilityAction::Activate_Implementation(UBlackboardComponent* BBComponent)
{
}

void UUtilityAction::Tick_Implementation(float DeltaTime)
{
}

void UUtilityAction::End_Implementation()
{
}
