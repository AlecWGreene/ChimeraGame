#include "ChimeraAbilitySystemComponent.h"

#include <ChimeraGAS/Public/ChimeraAbilitySystemGlobals.h>
#include <ChimeraGAS/Public/ChimeraGameplayAbility.h>

UChimeraAbilitySystemComponent::UChimeraAbilitySystemComponent()
{

}

FGameplayTag UChimeraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& Spec) const
{
	if (const UChimeraGameplayAbility* Ability = Cast<UChimeraGameplayAbility>(Spec.Ability))
	{
		return Ability->InputTag;
	}

	FGameplayTagContainer DynamicInputTags = Spec.DynamicAbilityTags.Filter(UChimeraAbilitySystemGlobals::Get().InputParentTag.GetSingleTagContainer());
	return DynamicInputTags.First();
}

void UChimeraAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bPaused)
{
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

	// Activate press abilities

}

void UChimeraAbilitySystemComponent::AbilityInput_Pressed(FGameplayTag InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
		{
			FGameplayTag SpecInputTag = GetInputTagFromSpec(Spec);
			if (SpecInputTag.IsValid() && SpecInputTag == InputTag)
			{
				InputPressedHandles.Add(Spec.Handle);
				InputHeldHandles.Add(Spec.Handle);
			}
		}
	}
}

void UChimeraAbilitySystemComponent::AbilityInput_Released(FGameplayTag InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
		{
			FGameplayTag SpecInputTag = GetInputTagFromSpec(Spec);
			if (SpecInputTag.IsValid() && SpecInputTag == InputTag)
			{
				InputReleasedHandles.Add(Spec.Handle);
				InputHeldHandles.Remove(Spec.Handle);
			}
		}
	}
}
