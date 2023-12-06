// Copyright Alec Greene. All Rights Reserved.
#include "Abilities/DeathAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include "ChimeraCharacter.h"
#include "VitalityTags.h"

UDeathAbility::UDeathAbility()
{
	ActivationPolicy = EAbilityActivationPolicy::OnAbilityTrigger;

	ActivationOwnedTags.AddTagFast(VitalityTags::State_Dead);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = VitalityTags::Event_Death_Start;
	AbilityTriggers.Add(TriggerData);
}

void UDeathAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// agreene 2023/12/5 - #ToDo #GAS may want to have cached chimera data like characters on abilities
	const UAnimMontage* DeathMontage = nullptr;
	if (AChimeraCharacter* Character = GetAvatar<AChimeraCharacter>())
	{
		DeathMontage = Character->GetMontageByTag(DeathMontageTag, DeathAnimSetTag);
	}

	if (DeathMontage)
	{
		// Not really sure why all the montage stuff requires a non-const ptr here, but since we don't have any game logic modifying it, its fine to remove const to pass it off to UE.
		UAbilityTask_PlayMontageAndWait* PlayMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
			"DeathMontageTask", 
			const_cast<UAnimMontage*>(DeathMontage));
		PlayMontage->OnCompleted.AddDynamic(this, &ThisClass::HandleMontageComplete);
		PlayMontage->ReadyForActivation();
	}
}

void UDeathAbility::PreEndAbility_Implementation(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo& ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->HandleGameplayEvent(VitalityTags::Event_Death_End, nullptr);
	}

	if (AActor* Avatar = GetAvatar<AActor>())
	{
		Avatar->Destroy();
	}
}

void UDeathAbility::PostEndAbility_Implementation(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo& ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
}

void UDeathAbility::HandleMontageComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
