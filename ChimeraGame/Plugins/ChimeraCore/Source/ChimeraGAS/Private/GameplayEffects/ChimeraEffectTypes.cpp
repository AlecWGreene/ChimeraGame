#include "GameplayEffects/ChimeraEffectTypes.h"

#include "AbilitySystemComponent.h"

// Copyright Alec Greene. All Rights Reserved.

FGameplayEffectSpecHandle FGameplayEffectSpecDef::CreateSpec(const UAbilitySystemComponent* ASC, FGameplayEffectContextHandle Context) const
{
	if (ASC)
	{
		FGameplayEffectContextHandle ContextToUse = Context;
		if (!ContextToUse.IsValid())
		{
			ContextToUse = ASC->MakeEffectContext();
		}

		FGameplayEffectSpecHandle Output = ASC->MakeOutgoingSpec(EffectClass, Level, ContextToUse);
		if (Output.IsValid())
		{
			Output.Data->SetLevel(Level);
			Output.Data->AppendDynamicAssetTags(DynamicAssetTags);
			Output.Data->DynamicGrantedTags.AppendTags(DynamicGrantedTags);
			Output.Data->SetByCallerTagMagnitudes.Append(SetByCallers);
			Output.Data->GrantedAbilitySpecs.Append(GrantedAbilities);

			TArray<FGameplayEffectSpecHandle> LinkedEffects = CreateLinkedEffectsList(ContextToUse, Output.Data->CapturedSourceTags.GetActorTags());
			Output.Data->TargetEffectSpecs.Append(LinkedEffects);
		}

		return Output;
	}

	return FGameplayEffectSpecHandle();
}

bool FGameplayEffectSpecDef::ModifySpec(FGameplayEffectSpecHandle& InOutSpec) const
{
	if (InOutSpec.IsValid())
	{
		InOutSpec.Data->SetLevel(Level);
		InOutSpec.Data->AppendDynamicAssetTags(DynamicAssetTags);
		InOutSpec.Data->DynamicGrantedTags.AppendTags(DynamicGrantedTags);
		InOutSpec.Data->SetByCallerTagMagnitudes.Append(SetByCallers);
		InOutSpec.Data->GrantedAbilitySpecs.Append(GrantedAbilities);

		TArray<FGameplayEffectSpecHandle> LinkedEffects = CreateLinkedEffectsList(
			InOutSpec.Data->GetEffectContext(), InOutSpec.Data->CapturedSourceTags.GetActorTags());
		InOutSpec.Data->TargetEffectSpecs.Append(LinkedEffects);
	}

	return false;
}

TArray<FGameplayEffectSpecHandle> FGameplayEffectSpecDef::CreateLinkedEffectsList(FGameplayEffectContextHandle Context, FGameplayTagContainer CapturedSourceTags) const
{
	if (Context.IsValid())
	{
		TArray<FGameplayEffectSpecHandle> Output;
		for (const FConditionalGameplayEffect& Effect : ConditionalEffects)
		{
			if (Effect.CanApply(CapturedSourceTags, Level))
			{
				FGameplayEffectSpecHandle SpecHandle = Effect.CreateSpec(Context, Level);
				if (SpecHandle.IsValid())
				{
					Output.Add(SpecHandle);
				}
			}
		}

		return Output;
	}

	return TArray<FGameplayEffectSpecHandle>();
}
