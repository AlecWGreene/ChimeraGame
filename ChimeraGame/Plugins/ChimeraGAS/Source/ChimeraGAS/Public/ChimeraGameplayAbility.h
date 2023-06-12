#pragma once

#include "Abilities/GameplayAbility.h"

#include "ChimeraGameplayAbility.generated.h"

UCLASS()
class CHIMERAGAS_API UChimeraGameplayAbility :
    public UGameplayAbility
{
    GENERATED_BODY()

    //----- Constructor and Engine Events -----//
public:
    UChimeraGameplayAbility();
};

