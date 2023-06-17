#include "ChimeraInputConfig.h"

#include "GameplayTagContainer.h"

const FChimeraInputLink& UChimeraInputConfig::GetNativeLink(FGameplayTag InputTag) const
{
    for (const FChimeraInputLink& InputLink : NativeLinks)
    {
        if (InputLink.InputTag == InputTag)
        {
            return InputLink;
        }
    }

    // @agreene #Todo - 2023/06/15 - Add custom log category
    UE_LOG(LogTemp, Warning, TEXT("Didn't find link in %s for %s"), *GetNameSafe(this), *InputTag.ToString());
    return FChimeraInputLink();
}

const TArray<FChimeraInputLink>& UChimeraInputConfig::GetAbliityLinks() const
{
    return AbilityLinks;
}
