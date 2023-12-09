// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask.h"

#include "ChimeraAbilitySystemComponent.h"

#include "AbilityTask_WaitInputEvent.generated.h"

UCLASS(BlueprintType)
class CHIMERAGAS_API UAbilityTask_WaitInputEvent : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks|Chimera", meta = (HidePin = "Owner", DefaultToSelf = "Owner", BlueprintInternalUseOnly = "true"))
	static UAbilityTask_WaitInputEvent* WaitInputEvent(
		UGameplayAbility* Owner,
		FName TaskInstanceName,
		const FGASInputEvent& InputEvent,
		bool bAllowInstantTrigger = false,
		bool bTriggerOnce = false);

	UPROPERTY(BlueprintAssignable)
	FGASInputEventDelegate OnInputEvent;

public:

	virtual void Activate() override;
	virtual void BeginDestroy() override;

protected:

	UFUNCTION()
	virtual void HandleInputEvent(const FInputActionInstance& InputActionInstance);

protected:
	bool bAllowInstantTrigger{ false };
	bool bTriggerOnce{ false };
	FGASInputEvent InputEvent;
};