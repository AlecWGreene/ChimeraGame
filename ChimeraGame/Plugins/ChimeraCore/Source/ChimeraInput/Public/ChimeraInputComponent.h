#pragma once

#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"

#include "ChimeraInputConfig.h"

#include "ChimeraInputComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;

UCLASS(BlueprintType)
class CHIMERAINPUT_API UChimeraInputComponent :
    public UEnhancedInputComponent
{
    GENERATED_BODY()

public:

    /**
    * Binds an InputAction, linked to the specified InputTag, to the provided function.
    * 
    * @param InputConfig Provides the InputAction -> InputTag associations
    * @param InputTag InputTag being bound to the function.
    * @param TriggerEvent The input event to which the function should respond to.
    * @param Object The object which owns the function being bound
    * @param Func The function being bound
    * 
    * @returns The binding handle of any succesfully bound handle, returns 0 if it failed.
    */
    template<class ObjectClass, typename FuncType>
    uint32 BindNativeAction(const UChimeraInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, ObjectClass* Object, FuncType Func);

    /**
    * Binds all InputActions specified as AbilityActions to the provided functions.
    *
    * @param InputConfig Provides the InputAction -> InputTag associations
    * @param Object The object which owns the function being bound
    * @param PressedFunc The function being bound
    * @param ReleasedFunc The function being bound
    * 
    * @returns The binding handle of any succesfully bound handle, returns 0 if it failed.
    */
    template<class ObjectClass, typename FuncType>
    TArray<uint32> BindAbilityActions(const UChimeraInputConfig* InputConfig, ObjectClass* Object, FuncType PressedFunc, FuncType ReleasedFunc);
};

template<class ObjectClass, typename FuncType>
uint32 UChimeraInputComponent::BindNativeAction(const UChimeraInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, ObjectClass* Object, FuncType Func)
{
    check(InputConfig);
    if (const UInputAction* InputAction = InputConfig->GetNativeAction(InputTag))
    {
        return BindAction(InputAction, TriggerEvent, Object, Func).GetHandle();
    }

    // @agreene #Todo - 2023/06/17 - Verify 0 is an invalid action handle
    return 0;
}

template<class ObjectClass, typename FuncType>
TArray<uint32> UChimeraInputComponent::BindAbilityActions(const UChimeraInputConfig* InputConfig, ObjectClass* Object, FuncType PressedFunc, FuncType ReleasedFunc)
{
    check(InputConfig);

    TArray<uint32> OutHandles;
    for (const FChimeraInputLink& InputLink : InputConfig->GetAbilityLinks())
    {
        if (InputLink.IsValid())
        {
            if (PressedFunc)
            {
                OutHandles.Add(BindAction(InputLink.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, InputLink.InputTag).GetHandle());
            }

            if (ReleasedFunc)
            {
                OutHandles.Add(BindAction(InputLink.InputAction, ETriggerEvent::Completed, Object, PressedFunc, InputLink.InputTag).GetHandle());
            }
        }
    }

    return OutHandles;
}