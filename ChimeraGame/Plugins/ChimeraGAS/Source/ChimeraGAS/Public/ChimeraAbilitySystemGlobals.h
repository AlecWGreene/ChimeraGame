#pragma once

#include "AbilitySystemGlobals.h"

#include "ChimeraAbilitySystemGlobals.generated.h"

UCLASS()
class CHIMERAGAS_API UChimeraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY();

public:

	static UChimeraAbilitySystemGlobals& Get() { return dynamic_cast<UChimeraAbilitySystemGlobals&>(UAbilitySystemGlobals::Get()); }

	FGameplayTag InputParentTag;
};