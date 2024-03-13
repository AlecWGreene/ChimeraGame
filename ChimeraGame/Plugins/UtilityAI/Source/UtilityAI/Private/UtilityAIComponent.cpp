// Copyright Alec Greene. All Rights Reserved.
#include "UtilityAIComponent.h"

#include "AIController.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"

#include "UtilityAction.h"

UUtilityAIComponent::UUtilityAIComponent()
{
	bWantsInitializeComponent = true;
}

void UUtilityAIComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (AAIController* Owner = GetOwner<AAIController>())
	{
		UBlackboardComponent* BBComponent = Owner->GetBlackboardComponent();
		Owner->UseBlackboard(Blackboard, BBComponent);

		Owner->BrainComponent = this;
		BlackboardComp = BBComponent;
	}

	for (TPair<FGameplayTag, TObjectPtr<class UUtilityAction>> ActionItem : Actions)
	{
		if (ensureMsgf(IsValid(ActionItem.Value), TEXT("Invalid action in %s under key %s"), *GetNameSafe(this), *ActionItem.Key.ToString()))
		{
			ActionItem.Value->Initialize(this);
		}
	}
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
		const TObjectPtr<UUtilityAction>* DesireActionPtr = Actions.Find(DesireData.Key);
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

void UUtilityAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateDesires(DeltaTime);

	if (ActiveAction == nullptr || !ActiveAction->IsActive())
	{
		float BestScore = 0.f;
		for (TPair<FGameplayTag, float> Desire : ActionDesires)
		{
			if (Desire.Value > BestScore)
			{
				TObjectPtr<UUtilityAction>* ActionPtr = Actions.Find(Desire.Key);
				if (ActionPtr && IsValid(*ActionPtr))
				{
					ActiveAction = *ActionPtr;
				}
			}
		}

		if (ActiveAction)
		{
			ActiveAction->Activate();
		}
	}
}

void UUtilityAIComponent::UpdateDesires(float DeltaTime)
{
	for (const TPair<FGameplayTag, UUtilityAction*>& ActionSlot : Actions)
	{
		if (IsValid(ActionSlot.Value))
		{
			float& Desire = ActionDesires.FindOrAdd(ActionSlot.Key);
			Desire = ActionSlot.Value->ComputeDesire(DeltaTime, Desire);
		}
	}
}