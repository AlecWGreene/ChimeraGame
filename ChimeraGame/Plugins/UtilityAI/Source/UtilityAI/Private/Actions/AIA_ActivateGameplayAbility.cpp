#include "Actions/AIA_ActivateGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

bool UAIA_ActivateGameplayAbility::CanActivate_Implementation() const
{
    return false;
}

void UAIA_ActivateGameplayAbility::Activate_Implementation()
{
    Super::Activate_Implementation();

    if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatar()))
    {
        FGameplayAbilitySpec* TargetSpec = nullptr;

        TArray<FGameplayAbilitySpec*> Candidates = FindAbilitySpecs();
        if (!Candidates.IsEmpty())
        {
            for (FGameplayAbilitySpec* Spec : Candidates)
            {
                if (Spec)
                {
                    UGameplayAbility* AbilityInstance = Spec->GetPrimaryInstance()
                        ? Spec->GetPrimaryInstance() 
                        : Spec->Ability.Get();
                    if (AbilityInstance && AbilityInstance->CanActivateAbility(Spec->Handle, ASC->AbilityActorInfo.Get(), nullptr, nullptr))
                    {
                        TargetSpec = Spec;
                    }
                }
            }
        }

        if (TargetSpec)
        {
            ASC->TryActivateAbility(TargetSpec->Handle);

            if (bWaitForAbilityEnd 
                && TargetSpec->Ability->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced 
                && TargetSpec->GetPrimaryInstance())
            {
                TargetSpec->GetPrimaryInstance()->OnGameplayAbilityEnded.AddUObject(this, &UAIA_ActivateGameplayAbility::HandleAbilityEnded);
            }
            // For abilities with no instance, we need to end immediately because these abilities cannot be stateful
            else
            {
                End(false);
                return;
            }
        }
        else
        {
            UE_LOG(LogUtilityAction, Error, TEXT("[UAIA_ActivateGameplayAbility::Activate] No abilities found."));
            End(true);
            return;
        }
    }
    else
    {
        UE_LOG(LogUtilityAction, Error, TEXT("[UAIA_ActivateGameplayAbility::Activate] Failed to get ASC."));
        End(true);
        return;
    }
}

void UAIA_ActivateGameplayAbility::End_Implementation(bool bCancelled)
{
    Super::End_Implementation(bCancelled);
}

TArray<FGameplayAbilitySpec*> UAIA_ActivateGameplayAbility::FindAbilitySpecs() const
{
    TArray<FGameplayAbilitySpec*> Output;

    if (bUseTagQuery && AbilityTagQuery.IsEmpty())
    {
        UE_LOG(LogUtilityAction, Error, TEXT("[UAIA_ActivateGameplayAbility::FindAbilitySpec] Empty tag query."));
        return Output;
    }
    else if (!bUseTagQuery && AbilityClass == nullptr)
    {
        UE_LOG(LogUtilityAction, Error, TEXT("[UAIA_ActivateGameplayAbility::FindAbilitySpec] Null ability class."));
        return Output;
    }

    if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatar()))
    {
        for (FGameplayAbilitySpec& AbilitySpec : ASC->GetActivatableAbilities())
        {
            if (bUseTagQuery)
            {
                if (AbilitySpec.Ability && AbilityTagQuery.Matches(AbilitySpec.Ability->AbilityTags))
                {
                    Output.Add(&AbilitySpec);
                }
            }
            else
            {
                if (AbilitySpec.Ability && AbilitySpec.Ability->IsA(AbilityClass))
                {
                    Output.Add(&AbilitySpec);
                }
            }
        }
    }
    else
    {
        UE_LOG(LogUtilityAction, Error, TEXT("[UAIA_ActivateGameplayAbility::FindAbilitySpec] Failed to get ASC."));
    }

    return Output;
}

void UAIA_ActivateGameplayAbility::HandleAbilityEnded(UGameplayAbility* AbilityInstance)
{
    End(false);
}
