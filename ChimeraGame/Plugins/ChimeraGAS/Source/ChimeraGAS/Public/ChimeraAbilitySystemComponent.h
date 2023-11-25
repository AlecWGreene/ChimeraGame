#pragma once

#include "AbilitySystemComponent.h"

#include "ChimeraAbilitySystemComponent.generated.h"

USTRUCT()
struct CHIMERAGAS_API FAbilityBufferWindow
{
    GENERATED_BODY()

    FAbilityBufferWindow();

    FGameplayTagContainer ListenTags;

    TArray<FGameplayAbilitySpecHandle> BufferedAbilities;
};

UCLASS()
class CHIMERAGAS_API UChimeraAbilitySystemComponent :
    public UAbilitySystemComponent
{
    GENERATED_BODY()

    //----- Overrides -----//
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

    TArray<FAbilityBufferWindow> ActiveBufferWindows;

    TArray<FGameplayAbilitySpecHandle> InputPressedHandles;
    TArray<FGameplayAbilitySpecHandle> InputHeldHandles;
    TArray<FGameplayAbilitySpecHandle> InputReleasedHandles;
};

