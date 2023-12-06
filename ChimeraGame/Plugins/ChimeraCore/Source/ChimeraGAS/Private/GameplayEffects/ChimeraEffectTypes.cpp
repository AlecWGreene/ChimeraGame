// Copyright Alec Greene. All Rights Reserved.
#include "GameplayEffects/ChimeraEffectTypes.h"

#include "AbilitySystemComponent.h"

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
	}

	return false;
}

bool FChimeraTargetDataFilter::FilterPassesForActor(const AActor* Actor) const
{
	if (const IGameplayTagAssetInterface* GameplayTagInterface = Cast<IGameplayTagAssetInterface>(Actor))
	{
		FGameplayTagContainer OwnedTags;
		GameplayTagInterface->GetOwnedGameplayTags(OwnedTags);

		if (!TargetTagConditions.Matches(OwnedTags))
		{
			return false;
		}
	}

	if ((FGenericTeamId::GetAttitude(SelfActor, Actor) & TargetAttitudes) == 0
		&& !(bOverrideTeamAttitudeForSelf && SelfActor == Actor))
	{
		return false;
	}

	return Super::FilterPassesForActor(Actor);
}
