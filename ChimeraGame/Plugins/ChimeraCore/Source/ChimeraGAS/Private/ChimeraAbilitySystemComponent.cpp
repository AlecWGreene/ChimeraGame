// Copyright Alec Greene. All Rights Reserved.
#include "ChimeraAbilitySystemComponent.h"

#include "ChimeraGAS\Public\ChimeraAbilitySystemGlobals.h"
#include "ChimeraGAS\Public\ChimeraGameplayAbility.h"
#include "ChimeraGAS\Public\ChimeraAbilitySystemGlobals.h"
#include "ChimeraGAS\Public\ChimeraGASLogCategories.h"

UChimeraAbilitySystemComponent::UChimeraAbilitySystemComponent()
{

}

FGameplayTag UChimeraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& Spec) const
{
	// This function usually gets called after every ProcessInput call on the input component, so we want a fast way to weed out abilities which don't respond to inputs.
	const UChimeraGameplayAbility* Ability = Cast<UChimeraGameplayAbility>(Spec.Ability);
	if (Ability && !(Ability->ActivationPolicy == EAbilityActivationPolicy::OnInputPressed || Ability->ActivationPolicy == EAbilityActivationPolicy::WhileInputHeld))
	{
		return FGameplayTag::EmptyTag;
	}

	// Allow any dynamic ability tags to override the CDO
	FGameplayTagContainer DynamicInputTags = Spec.DynamicAbilityTags.Filter(UChimeraAbilitySystemGlobals::Get().InputParentTag.GetSingleTagContainer());
	if (!DynamicInputTags.IsEmpty())
	{
		return DynamicInputTags.First();
	}

	if (Ability)
	{
		return Ability->InputTag;
	}

	return FGameplayTag::EmptyTag;
}

void UChimeraAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bPaused)
{
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldHandles)
	{
		FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(SpecHandle);
		if (Spec && !Spec->IsActive())
		{
			const UChimeraGameplayAbility* CDO = Cast<UChimeraGameplayAbility>(Spec->Ability);
			if (CDO && CDO->ActivationPolicy == EAbilityActivationPolicy::WhileInputHeld)
			{
				AbilitiesToActivate.Add(SpecHandle);
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedHandles)
	{
		if (FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (Spec->IsActive())
			{
				Spec->InputPressed = true;
				AbilitySpecInputPressed(*Spec);
			}
			else
			{
				const UChimeraGameplayAbility* CDO = Cast<UChimeraGameplayAbility>(Spec->Ability);
				if (CDO && CDO->ActivationPolicy == EAbilityActivationPolicy::OnInputPressed)
				{
					AbilitiesToActivate.Add(SpecHandle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(SpecHandle);
	}

	// We process InputReleasedHandles last to cover the case of pressing/releasing an input on same frame
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedHandles)
	{
		if (FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(SpecHandle))
		{
			Spec->InputPressed = false;

			if (Spec->IsActive())
			{
				AbilitySpecInputReleased(*Spec);
			}
		}
	}

	InputPressedHandles.Reset();
	InputReleasedHandles.Reset();
}

void UChimeraAbilitySystemComponent::AbilityInput_Pressed(FGameplayTag InputTag)
{
	if (InputTag.IsValid())
	{
		UE_LOG(LogChimeraAbilityInput, Verbose, TEXT("%s pressed"), *InputTag.ToString());

		for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
		{
			FGameplayTag SpecInputTag = GetInputTagFromSpec(Spec);
			if (SpecInputTag.IsValid() && SpecInputTag == InputTag)
			{
				InputPressedHandles.AddUnique(Spec.Handle);
				InputHeldHandles.AddUnique(Spec.Handle);
			}
		}
	}
}

void UChimeraAbilitySystemComponent::AbilityInput_Released(FGameplayTag InputTag)
{
	if (InputTag.IsValid())
	{
		UE_LOG(LogChimeraAbilityInput, Verbose, TEXT("%s released"), *InputTag.ToString());

		for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
		{
			FGameplayTag SpecInputTag = GetInputTagFromSpec(Spec);
			if (SpecInputTag.IsValid() && SpecInputTag == InputTag)
			{
				InputReleasedHandles.AddUnique(Spec.Handle);
				InputHeldHandles.Remove(Spec.Handle);
			}
		}
	}
}
