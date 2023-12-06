#include "ChimeraGASFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "ChimeraAbilitySystemComponent.h"

UChimeraAbilitySystemComponent* UChimeraGASFunctionLibrary::GetChimeraASC(const AActor* Actor)
{
	return GetASCFromActor<UChimeraAbilitySystemComponent>(Actor);
}

FGameplayEffectSpecHandle UChimeraGASFunctionLibrary::MakeSpecFromDef(const UAbilitySystemComponent* ASC, FGameplayEffectContextHandle Context, const FGameplayEffectSpecDef& SpecDef)
{
	return SpecDef.CreateSpec(ASC, Context);
}

bool UChimeraGASFunctionLibrary::TargetDataFilterPassesForActor(const AActor* Actor, const FChimeraTargetDataFilter& Filter)
{
	return Filter.FilterPassesForActor(Actor);
}
