#include "ChimeraAbilitySystemComponent.h"

#include "EnhancedInputComponent.h"

#include "ChimeraAbilitySystemGlobals.h"
#include "ChimeraAttributeSet.h"
#include "ChimeraGameplayAbility.h"

UChimeraAbilitySystemComponent::UChimeraAbilitySystemComponent()
{

}

void UChimeraAbilitySystemComponent::BindToInputComponent(UInputComponent* InputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputCompoent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		CachedInputComponent = EnhancedInputCompoent;

		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			BindAbilityInput(AbilitySpec);
		}
	}
}

void UChimeraAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);
	if (AbilitySpec.Handle.IsValid())
	{
		BindAbilityInput(AbilitySpec);
	}
}

void UChimeraAbilitySystemComponent::BindAbilityInput(const FGameplayAbilitySpec& Spec)
{
	const UChimeraGameplayAbility* Ability = Cast<UChimeraGameplayAbility>(Spec.Ability);
	if (Ability
		&& Ability->ActivationPolicy == EAbilityActivationPolicy::OnInput
		&& Ability->ActivationEvent.IsValid())
	{
		if (TSet<FGameplayAbilitySpecHandle>* AbilitySet = AbilityInputActivations.Find(Ability->ActivationEvent))
		{
			AbilitySet->Add(Spec.Handle);
		}
		else if (CachedInputComponent.IsValid())
		{
			// agreene 2023/11/28 - #ToDo #Input I may want to look at unbinding from these events, but it shouldn't matter for now.
			CachedInputComponent->BindAction(
				Ability->ActivationEvent.InputAction,
				Ability->ActivationEvent.TriggerEvent,
				this, &ThisClass::HandleInputEvent);

			CachedInputComponent->BindActionValue(Ability->ActivationEvent.InputAction);

			TSet<FGameplayAbilitySpecHandle>& NewAbilitySet = AbilityInputActivations.Add(Ability->ActivationEvent);
			NewAbilitySet.Add(Spec.Handle);
		}
	}
}

void UChimeraAbilitySystemComponent::HandleInputEvent(const FInputActionInstance& InputActionInstance)
{
	FGASInputEvent InputEvent(InputActionInstance.GetSourceAction(), InputActionInstance.GetTriggerEvent());
	if (TSet<FGameplayAbilitySpecHandle>* AbilitySet = AbilityInputActivations.Find(InputEvent))
	{
		for (FGameplayAbilitySpecHandle AbilitySpecHandle : *AbilitySet)
		{
			TryActivateAbility(AbilitySpecHandle);
		}
	}
}

const UAttributeSet* UChimeraAbilitySystemComponent::ApplyInitializer(const UChimeraAttributeSetInitializer* Initializer)
{
	if (IsValid(Initializer))
	{
		const UAttributeSet* Output = GetOrCreateAttributeSubobject(Initializer->AttributeSetClass);
		if (IsValid(Output))
		{
			Initializer->InitializeAttributeSet(const_cast<UAttributeSet*>(Output));
		}

		return Output;
	}

	return nullptr;
}
