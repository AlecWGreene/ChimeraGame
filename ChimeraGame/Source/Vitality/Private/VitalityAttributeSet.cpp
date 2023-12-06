// Copyright Alec Greene. All Rights Reserved.
#include "VitalityAttributeSet.h"

#include "GameplayEffectExtension.h"

#include "VitalityTags.h"

UVitalityAttributeSet::UVitalityAttributeSet()
{
}

void UVitalityAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{

}

void UVitalityAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		if (NewValue <= 0.f)
		{
			if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
			{
				// agreene 2023/12/5 - #ToDo #Vitality Populate death data
				FGameplayEventData DeathEventData;

				ASC->HandleGameplayEvent(VitalityTags::Event_Death_Start, &DeathEventData);
			}
		}
	}
}

void UVitalityAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// Handle meta attribute changes
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		HandleDamage(Data.EffectSpec, Data.EvaluatedData.Magnitude);
	}
	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		HandleHealing(Data.EffectSpec, Data.EvaluatedData.Magnitude);
	}
}

void UVitalityAttributeSet::HandleDamage(const FGameplayEffectSpec& Spec, float Amount)
{
	const float OldHealth = Health.GetBaseValue();

	float NewHealth = OldHealth - Amount;

	SetHealth(NewHealth);
}

void UVitalityAttributeSet::HandleHealing(const FGameplayEffectSpec& Spec, float Amount)
{
	const float OldHealth = Health.GetBaseValue();

	float NewHealth = OldHealth + Amount;

	SetHealth(NewHealth);
}
