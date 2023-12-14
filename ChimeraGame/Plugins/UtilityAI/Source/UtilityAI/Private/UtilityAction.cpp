// Copyright Alec Greene. All Rights Reserved.
#include "UtilityAction.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameplayTasksComponent.h"
#include "Tasks/AITask.h"

#include "UtilityAIComponent.h"

UUtilityAction::UUtilityAction()
{
}

UGameplayTasksComponent* UUtilityAction::GetGameplayTasksComponent(const UGameplayTask& Task) const
{
	const UAITask* AITask = Cast<UAITask>(&Task);
	if (IsValid(AITask) && IsValid(AITask->GetAIController()))
	{
		return AITask->GetAIController()->GetGameplayTasksComponent();
	}

	return nullptr;
}

AActor* UUtilityAction::GetGameplayTaskOwner(const UGameplayTask* Task) const
{
	if (Task)
	{
		if (const UGameplayTasksComponent* Component = GetGameplayTasksComponent(*Task))
		{
			return Component->GetOwner();
		}
	}

	return nullptr;
}

AActor* UUtilityAction::GetGameplayTaskAvatar(const UGameplayTask* Task) const
{
	const UAITask* AITask = Cast<UAITask>(Task);
	if (IsValid(AITask) && IsValid(AITask->GetAIController()))
	{
		return AITask->GetAIController()->GetPawn();
	}

	return nullptr;
}

void UUtilityAction::OnGameplayTaskInitialized(UGameplayTask& Task)
{
	// agreene 2023/12/13 - #ToDo #UtilityAI If we make a custom GameplayTask class, this 
	// is where we would initialize references or other custom setup data.
}

void UUtilityAction::OnGameplayTaskActivated(UGameplayTask& Task)
{
	ActiveTasks.Add(&Task);
}

void UUtilityAction::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	ActiveTasks.Remove(&Task);
}

void UUtilityAction::Initialize_Implementation(class UUtilityAIComponent* UtilityComponent)
{
	check(UtilityComponent && UtilityComponent->GetOwner())
	CachedUtilityComponent = UtilityComponent;
	CachedController = Cast<AAIController>(UtilityComponent->GetOwner());
	Avatar = CachedController->GetPawn();
}

float UUtilityAction::ComputeDesire_Implementation(float DeltaTime, float PreviousDesire) const
{
	return 0.0f;
}

bool UUtilityAction::CanActivate_Implementation() const
{
	return false;
}

void UUtilityAction::Activate_Implementation()
{
	bActive = true;
}

void UUtilityAction::Tick_Implementation(float DeltaTime)
{
}

void UUtilityAction::End_Implementation(bool bCancelled)
{
	bActive = false;

	for (UGameplayTask* Task : ActiveTasks)
	{
		if (Task && Task->IsActive())
		{
			Task->EndTask();
		}
	}

	ActiveTasks.Empty();
}

UUtilityAIComponent* UUtilityAction::GetUtilityComponent() const
{
	return CachedUtilityComponent.Get();
}

UBlackboardComponent* UUtilityAction::GetBlackboardComponent() const
{
	return CachedUtilityComponent.IsValid() ? CachedUtilityComponent->GetBlackboardComponent() : nullptr;
}

AAIController* UUtilityAction::GetController() const
{
	return CachedController.Get();
}

APawn* UUtilityAction::GetAvatar() const
{
	return CachedController.IsValid() ? CachedController->GetPawn() : nullptr;
}
