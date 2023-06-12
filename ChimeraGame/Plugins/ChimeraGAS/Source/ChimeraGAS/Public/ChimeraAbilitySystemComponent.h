#pragma once

#include "AbilitySystemComponent.h"

#include "ChimeraAbilitySystemComponent.generated.h"

UCLASS()
class CHIMERAGAS_API UChimeraAbilitySystemComponent :
    public UAbilitySystemComponent
{
    GENERATED_BODY()

    //----- Constructor and Engine Events -----//
public:
    UChimeraAbilitySystemComponent();
};

