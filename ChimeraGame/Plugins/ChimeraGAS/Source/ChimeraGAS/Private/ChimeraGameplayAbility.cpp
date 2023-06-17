#include "ChimeraGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "..\Public\ChimeraGameplayAbility.h"

UChimeraGameplayAbility::UChimeraGameplayAbility()
{

}

/////////////////////////////////////////////////////////////////////
//	UGameplayAbility Overrides
/////////////////////////////////////////////////////////////////////

bool UChimeraGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bSuperCanActivate = Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);

	if (IsInstantiated())
	{
		check(ActorInfo);

		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		check(ASC);

		const FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Handle);
		checkf(Spec, TEXT("Invalid ability spec for handle %s"), *Handle.ToString())

			if (const UChimeraGameplayAbility* AbilityInstance = Cast<UChimeraGameplayAbility>(Spec->GetPrimaryInstance()))
			{
				FGameplayTagContainer LocalSourceTags;
				if (SourceTags)
				{
					LocalSourceTags.AppendTags(*SourceTags);
				}

				FGameplayTagContainer LocalTargetTags;
				if (TargetTags)
				{
					LocalTargetTags.AppendTags(*TargetTags);
				}

				FGameplayTagContainer OutTags;
				bool bInstanceCanActivate = AbilityInstance->CanActivateInstance(LocalSourceTags, LocalTargetTags, OutTags);
				if (OptionalRelevantTags)
				{
					OptionalRelevantTags->AppendTags(OutTags);
				}

				return bSuperCanActivate && bInstanceCanActivate;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Could not get primary ability instance for handle %s"), *Handle.ToString());
			}
	}

	return bSuperCanActivate;
}

void UChimeraGameplayAbility::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	if (IsInstantiated())
	{
		if (TriggerEventData)
		{
			CachedEventData = *TriggerEventData;
		}
	}
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
			UE_LOG(LogAbilitySystem, Warning, TEXT("Ability %s expects event data but none is being supplied. Use Activate Ability instead of Activate Ability From Event."), *GetName());
			bool bReplicateEndAbility = false;
			bool bWasCancelled = true;
			EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		}
	}

	// @agreene #ToDo - 2023/07/12 - Custom logic will go in here, but not sure how to handle the BP ActivateAbility pipeline.
	if (IsInstantiated())
	{
		ActivateInstance();
	}
}

bool UChimeraGameplayAbility::CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bSuperCooldown = Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);

	if (IsInstantiated())
	{
		check(ActorInfo);

		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		check(ASC);

		const FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Handle);
		checkf(Spec, TEXT("Invalid ability spec for handle %s"), *Handle.ToString())

			if (const UChimeraGameplayAbility* AbilityInstance = Cast<UChimeraGameplayAbility>(Spec->GetPrimaryInstance()))
			{
				FGameplayTagContainer OutTags;
				bool bInstanceCooldown = AbilityInstance->CheckInstanceCooldown(OutTags);
				if (OptionalRelevantTags)
				{
					OptionalRelevantTags->AppendTags(OutTags);
				}

				return bSuperCooldown && bInstanceCooldown;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Could not get primary ability instance for handle %s"), *Handle.ToString());
			}
	}

	return bSuperCooldown;
}

void UChimeraGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);

	if (IsInstantiated())
	{
		ApplyInstanceCooldown();
	}
}

bool UChimeraGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bSuperCost = Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);

	if (IsInstantiated())
	{
		check(ActorInfo);

		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		check(ASC);

		const FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Handle);
		checkf(Spec, TEXT("Invalid ability spec for handle %s"), *Handle.ToString())

		if (const UChimeraGameplayAbility* AbilityInstance = Cast<UChimeraGameplayAbility>(Spec->GetPrimaryInstance()))
		{
			FGameplayTagContainer OutTags;
			bool bInstanceCost = AbilityInstance->CheckInstanceCost(OutTags);
			if (OptionalRelevantTags)
			{
				OptionalRelevantTags->AppendTags(OutTags);
			}

			return bSuperCost && bInstanceCost;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Could not get primary ability instance for handle %s"), *Handle.ToString());
		}
	}
	
	return bSuperCost;
}

void UChimeraGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);

	if (IsInstantiated())
	{
		ApplyInstanceCost();
	}
}

void UChimeraGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	// @agreene #ToDo - 2023/06/13 - Is this necessary or is just EndAbility enough?
	// Give the instance a chance to do any cancel specific cleanup
	if (IsInstantiated())
	{
		CancelInstance();
	}

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

		EndInstance(bWasCancelled);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

/////////////////////////////////////////////////////////////////////
//	Instance Lifecycle
/////////////////////////////////////////////////////////////////////

bool UChimeraGameplayAbility::CanActivateInstance_Implementation(const FGameplayTagContainer& SourceTags, const FGameplayTagContainer& TargetTags, FGameplayTagContainer& OptionalRelevantTags) const
{
	return true;
}

void UChimeraGameplayAbility::ActivateInstance_Implementation()
{
}

bool UChimeraGameplayAbility::CheckInstanceCooldown_Implementation(FGameplayTagContainer& OptionalRelevantTags) const
{
	return true;
}

void UChimeraGameplayAbility::ApplyInstanceCooldown_Implementation() const
{
}

bool UChimeraGameplayAbility::CheckInstanceCost_Implementation(FGameplayTagContainer& OptionalRelevantTags) const
{
	return true;
}

void UChimeraGameplayAbility::ApplyInstanceCost_Implementation() const
{
}

void UChimeraGameplayAbility::CancelInstance_Implementation()
{
}

void UChimeraGameplayAbility::EndInstance_Implementation(bool bWasCancelled)
{
}

void UChimeraGameplayAbility::PostEndInstance_Implementation(bool bWasCancelled)
{
}
