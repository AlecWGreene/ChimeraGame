#include "ChimeraGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "ChimeraGAS/Public/ChimeraGameplayAbility.h"
#include "ChimeraGAS/Public/ChimeraAbilitySystemGlobals.h"
#include "ChimeraGAS/Public/ChimeraGASLogCategories.h"

UChimeraGameplayAbility::UChimeraGameplayAbility()
{

}

/////////////////////////////////////////////////////////////////////
//	UGameplayAbility Overrides
/////////////////////////////////////////////////////////////////////

bool UChimeraGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);
}

void UChimeraGameplayAbility::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UChimeraGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// @agreene #Note - 2023/07/12 - Coped from UGameplayAbility::ActivateAbility and modified so CommitAbility isn't automatically called.
	if (bHasBlueprintActivate)
	{
		// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
		K2_ActivateAbility();
	}
	else if (bHasBlueprintActivateFromEvent)
	{
		if (TriggerEventData)
		{
			// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
			K2_ActivateAbilityFromEvent(*TriggerEventData);
		}
		else
		{
			UE_LOG(LogChimeraAbility, Warning, TEXT("Ability %s expects event data but none is being supplied. Use Activate Ability instead of Activate Ability From Event."), *GetName());
			bool bReplicateEndAbility = false;
			bool bWasCancelled = true;
			EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		}
	}

	// @agreene #ToDo - 2023/07/12 - Custom logic will go in here, but not sure how to handle the BP ActivateAbility pipeline.
}

bool UChimeraGameplayAbility::CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);
}

void UChimeraGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
}

bool UChimeraGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
}

void UChimeraGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
}

void UChimeraGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UChimeraGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// If the scope is locked, we early out since the super call will automatically add the EndAbility call to a queue
	// which is executed after the lock is lifted.
	if (ScopeLockCount > 0)
	{
		return;
	}

	if (IsInstantiated())
	{
		// If tasks or other delegates are bound to EndAbility, it can get called twice.
		if (!IsEndAbilityValid(Handle, ActorInfo))
		{
			return;
		}
	}

	FGameplayAbilityActorInfo AbilityActorInfo = ActorInfo ? *ActorInfo : FGameplayAbilityActorInfo();

	PreEndAbility(Handle, AbilityActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	PostEndAbility(Handle, AbilityActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}