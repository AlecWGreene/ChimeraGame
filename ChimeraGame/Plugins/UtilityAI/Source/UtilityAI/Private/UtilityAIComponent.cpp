// Copyright Alec Greene. All Rights Reserved.
#include "UtilityAIComponent.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"

#include "Actions/UtilityAction.h"
#include "Actions/UtilityActionSet.h"

DEFINE_LOG_CATEGORY_STATIC(LogUtilityAIComponent, Log, All);

UUtilityAIComponent::UUtilityAIComponent()
{
	bWantsInitializeComponent = true;
}

void UUtilityAIComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (AAIController* Owner = GetOwner<AAIController>())
	{
		Owner->BrainComponent = this;
	}

	if (ActionSet)
	{
		UpdateActionSet(ActionSet);
	}
}

void UUtilityAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bLogicRunning)
	{
		// agreene 2024/03/19 - #ToDo #UtilityAI Sort out how and when desires are updated
		UpdateDesires(DeltaTime);

		// agreene 2024/03/19 - #ToDo #UtilityAI While initialization pipeline is under construction, try to fix situations in which we're doing nothing
		if (!ActiveAction || !ActiveAction->bActive)
		{
			SelectNewAction();
		}
	}
}

bool UUtilityAIComponent::IsRunning() const
{
	return ActiveAction != nullptr;
}

bool UUtilityAIComponent::IsPaused() const
{
	return false;
}

void UUtilityAIComponent::StartLogic()
{
	bLogicRunning = true;

	if (ActiveAction)
	{
		ActiveAction->Activate();
	}
	else
	{
		SelectNewAction();
	}
}

void UUtilityAIComponent::PauseLogic(const FString& Reason)
{

}

void UUtilityAIComponent::RestartLogic()
{
}

void UUtilityAIComponent::StopLogic(const FString& Reason)
{
	bLogicRunning = false;

	if (ActiveAction)
	{
		ActiveAction->End(true);
		ActiveAction = nullptr;
	}
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
		const TObjectPtr<UUtilityAction>* DesireActionPtr = AvailableActions.Find(DesireData.Key);
		if (DesireActionPtr && *DesireActionPtr == ActiveAction)
		{
			Output += FString::Printf(TEXT("\t%s = %3f [Active]\n"), *DesireData.Key.ToString(), DesireData.Value);
			for (const UGameplayTask* Task : ActiveAction->ActiveTasks)
			{
				Output += FString::Printf(TEXT("\t\t%s\n"), *GetNameSafe(Task));
			}
		}
		else
		{
			Output += FString::Printf(TEXT("\t%s = %3f\n"), *DesireData.Key.ToString(), DesireData.Value);
		}
	}

	return Output;
}

void UUtilityAIComponent::UpdateActionSet(UUtilityActionSet* InActionSet)
{
	// Cleanup old ActionSet
	if (ActionSet)
	{
		for (const TPair<FGameplayTag, UUtilityAction*>& ActionSetSlot : ActionSet->Actions)
		{
			AvailableActions.Remove(ActionSetSlot.Key);
		}
	}

	ActionSet = InActionSet;

	// Add new ActionSet
	if (InActionSet)
	{
		if (InActionSet->Blackboard)
		{
			if (AAIController* Owner = GetOwner<AAIController>())
			{
				UBlackboardComponent* BBComponent = Owner->GetBlackboardComponent();
				Owner->UseBlackboard(InActionSet->Blackboard, BBComponent);

				BlackboardComp = BBComponent;
			}
		}

		const float DeltaTime = GetWorld()->DeltaTimeSeconds;
		for (const TPair<FGameplayTag, UUtilityAction*>& ActionSetSlot : InActionSet->Actions)
		{
			if (!AvailableActions.Contains(ActionSetSlot.Key))
			{
				if (ensureMsgf(IsValid(ActionSetSlot.Value), TEXT("Invalid action in %s under key %s"), *GetNameSafe(this), *ActionSetSlot.Key.ToString()))
				{
					UUtilityAction* ActionInstance = NewObject<UUtilityAction>(this, ActionSetSlot.Value->GetClass(), NAME_None, RF_Transient, ActionSetSlot.Value);
					if (ActionInstance)
					{
						ActionInstance->Initialize(this);
						AvailableActions.Add(ActionSetSlot.Key, ActionInstance);
					}
				}
			}
			else
			{
				UE_LOG(LogUtilityAIComponent, Warning, TEXT("Attempted to add %s for ActionSet %s, but key is already in use."), 
					*ActionSetSlot.Key.ToString(), *GetNameSafe(InActionSet));
			}
		}
	}
}

void UUtilityAIComponent::UpdateDesires(float DeltaTime)
{
	for (const TPair<FGameplayTag, UUtilityAction*>& ActionSlot : AvailableActions)
	{
		if (IsValid(ActionSlot.Value))
		{
			float& Desire = ActionDesires.FindOrAdd(ActionSlot.Key);
			Desire = ActionSlot.Value->ComputeDesire(DeltaTime, Desire);
		}
	}
}

void UUtilityAIComponent::SelectNewAction()
{
	if (!ActionOverrides.IsEmpty())
	{
		// agreene 2024/03/19 - #ToDo #UtilityAI Implement override priority
		for (FGameplayTag OverrideKey : ActionOverrides)
		{
			if (TObjectPtr<UUtilityAction>* ActionPtr = AvailableActions.Find(OverrideKey))
			{
				if (*ActionPtr && (*ActionPtr)->CanActivate())
				{
					if (*ActionPtr)
					{
						ActiveAction = *ActionPtr;
						ActiveAction->Activate();
						return;
					}
				}
			}
		}
	}
	
	UUtilityAction* SelectedAction = nullptr;
	float BestDesire = -1.f;
	for (const TPair<FGameplayTag, float>& Desire : ActionDesires)
	{
		if (BestDesire < Desire.Value)
		{
			TObjectPtr<UUtilityAction>* ActionPtr = AvailableActions.Find(Desire.Key);
			if (ActionPtr && *ActionPtr && (*ActionPtr)->CanActivate())
			{
				SelectedAction = *ActionPtr;
				BestDesire = Desire.Value;
			}
		}
	}

	if (SelectedAction)
	{
		ActiveAction = SelectedAction;
		ActiveAction->Activate();
	}
	else
	{
		UE_LOG(LogUtilityAIComponent, Warning, TEXT("Failed to select new action."));
	}
}