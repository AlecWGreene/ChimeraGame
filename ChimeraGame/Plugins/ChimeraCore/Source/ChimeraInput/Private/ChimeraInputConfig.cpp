#include "ChimeraInputConfig.h"
#include "InputAction.h"
#include "GameplayTagContainer.h"

const UInputAction* UChimeraInputConfig::GetNativeAction(FGameplayTag InputTag) const
{
    for (const FChimeraInputLink& InputLink : NativeLinks)
    {
        if (InputLink.InputTag == InputTag)
        {
            return InputLink.InputAction;
        }
    }

    // @agreene #Todo - 2023/06/15 - Add custom log category
    UE_LOG(LogTemp, Warning, TEXT("Didn't find link in %s for %s"), *GetNameSafe(this), *InputTag.ToString());
    return nullptr;
}

const TArray<FChimeraInputLink>& UChimeraInputConfig::GetAbilityLinks() const
{
    return AbilityLinks;
}
