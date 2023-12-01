// Copyright Alec Greene. All Rights Reserved.
#include "UtilityAIComponent.h"

#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"

#include "UtilityAction.h"
#include "UtilityFactors.h"

UUtilityAIComponent::UUtilityAIComponent()
{
}

void UUtilityAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateFacts(DeltaTime);

	UpdateDesires(DeltaTime);
}

void UUtilityAIComponent::UpdateFacts(float DeltaTime)
{
	for (const TPair<FGameplayTag, UUtilityFactCalculator*>& Calculator : FactCalculators)
	{
		if (IsValid(Calculator.Value))
		{
			FVector OldValue = FVector::ZeroVector;
			Factors.GetFact(Calculator.Key, OldValue);

			Factors.SetOrAddFact(Calculator.Key, Calculator.Value->CalculateValue(DeltaTime, OldValue, Factors));
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
			Desire = ActionSlot.Value->ComputeDesire(DeltaTime, Desire, Factors);
		}
	}
}

void UUtilityAIComponent::UpdateReference(FGameplayTag ReferenceKey, const UObject* Object)
{
	Factors.SetOrAddReference(ReferenceKey, Object);
}

void UUtilityAIComponent::OnDisplayDebug(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float YL, float YPos)
{
	static const FName NAME_UtilityAI = "UtilityAI";
	if (!HUD || !Canvas || !HUD->ShouldDisplayDebug(NAME_UtilityAI))
	{
		return;
	}

	if (AActor* DebugTarget = HUD->GetCurrentDebugTargetActor())
	{
		UUtilityAIComponent* DebugComponent = nullptr;
		if (APawn* DebugPawn = Cast<APawn>(DebugTarget))
		{
			if (AController* Controller = DebugPawn->GetController())
			{
				DebugComponent = Controller->FindComponentByClass<UUtilityAIComponent>();
			}
		}
		
		if (DebugComponent == nullptr)
		{
			DebugComponent = DebugTarget->FindComponentByClass<UUtilityAIComponent>();
		}

		if (DebugComponent)
		{
			DebugComponent->DisplayDebug(HUD, Canvas, DebugDisplay, YL, YPos);
		}
	}
}

void UUtilityAIComponent::DisplayDebug(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float YL, float YPos)
{
	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	// Display References
	DisplayDebugManager.SetDrawColor(FColor::White);
	DisplayDebugManager.DrawString(TEXT("References"));
	DisplayDebugManager.SetDrawColor(FColor(67,67,67));
	for (FGameplayTag Key : Factors.GetReferenceKeys())
	{
		const UObject* ReferenceValue = nullptr;
		Factors.GetReference(Key, ReferenceValue);

		DisplayDebugManager.DrawString(FString::Printf(TEXT("\t%s = %s"), *Key.ToString(), *GetNameSafe(ReferenceValue)));
	}

	// Display Facts
	DisplayDebugManager.SetDrawColor(FColor::White);
	DisplayDebugManager.DrawString(TEXT("References"));
	DisplayDebugManager.SetDrawColor(FColor(67, 67, 67));
	for (FGameplayTag Key : Factors.GetFactKeys())
	{
		FVector FactValue;
		Factors.GetFact(Key, FactValue);

		DisplayDebugManager.DrawString(FString::Printf(TEXT("\t%s = %s"), *Key.ToString(), *FactValue.ToCompactString()));
	}

	// Display Actions
	DisplayDebugManager.SetDrawColor(FColor::White);
	DisplayDebugManager.DrawString(TEXT("Actions"));
	DisplayDebugManager.SetDrawColor(FColor(67, 67, 67));
	for (TPair<FGameplayTag, float> DesireData : ActionDesires)
	{
		DisplayDebugManager.DrawString(FString::Printf(TEXT("\t%s = %f"), *DesireData.Key.ToString(), DesireData.Value));
	}
}
