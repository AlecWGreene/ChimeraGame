// Copyright Alec Greene. All Rights Reserved.

#include "Abilities/Tasks/AbilityTask_WaitInputEvent.h"

UAbilityTask_WaitInputEvent* UAbilityTask_WaitInputEvent::WaitInputEvent(
    UGameplayAbility* Owner, 
    FName TaskInstanceName, 
    const FGASInputEvent& InputEvent, 
    bool bAllowInstantTrigger, 
    bool bTriggerOnce)
{
    UAbilityTask_WaitInputEvent* NewTask = NewAbilityTask<UAbilityTask_WaitInputEvent>(Owner, TaskInstanceName);
    NewTask->InputEvent = InputEvent;
    NewTask->bAllowInstantTrigger = bAllowInstantTrigger;
    NewTask->bTriggerOnce = bTriggerOnce;

    return NewTask;
}

void UAbilityTask_WaitInputEvent::Activate()
{
    if (AbilitySystemComponent.IsValid())
    {
        if (UChimeraAbilitySystemComponent* ASC = Cast<UChimeraAbilitySystemComponent>(AbilitySystemComponent))
        {
            ASC->FindOrAddGASInputEventDelegate(InputEvent).AddDynamic(this, &UAbilityTask_WaitInputEvent::HandleInputEvent);
        }
    }
}

void UAbilityTask_WaitInputEvent::BeginDestroy()
{
    if (AbilitySystemComponent.IsValid())
    {
        if (UChimeraAbilitySystemComponent* ASC = Cast<UChimeraAbilitySystemComponent>(AbilitySystemComponent))
        {
            ASC->FindOrAddGASInputEventDelegate(InputEvent).RemoveDynamic(this, &UAbilityTask_WaitInputEvent::HandleInputEvent);
        }
    }

    Super::BeginDestroy();
}

void UAbilityTask_WaitInputEvent::HandleInputEvent(const FInputActionInstance& InputActionInstance)
{
    if (ShouldBroadcastAbilityTaskDelegates() && OnInputEvent.IsBound())
    {
        OnInputEvent.Broadcast(InputActionInstance);
    }

    if (bTriggerOnce)
    {
        EndTask();
    }
}
