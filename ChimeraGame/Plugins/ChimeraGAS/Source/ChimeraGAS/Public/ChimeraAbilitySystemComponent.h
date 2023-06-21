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

    //----- Input -----//
public:

    virtual FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& Spec) const;
    virtual void ProcessAbilityInput(float DeltaTime, bool bPaused);
    virtual void AbilityInput_Pressed(FGameplayTag InputTag);
    virtual void AbilityInput_Released(FGameplayTag InputTag);

    //----- Class Properties -----//
protected:

    //----- Instance Variables -----//
public:

    TArray<FGameplayAbilitySpecHandle> InputPressedHandles;
    TArray<FGameplayAbilitySpecHandle> InputHeldHandles;
    TArray<FGameplayAbilitySpecHandle> InputReleasedHandles;
};

