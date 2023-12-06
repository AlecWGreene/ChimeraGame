// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace ChimeraCoreTags
{
    CHIMERACORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag);
    CHIMERACORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
    CHIMERACORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);

    CHIMERACORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Walking);
    CHIMERACORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_NavWalking);
    CHIMERACORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Falling);
    CHIMERACORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Flying);
    CHIMERACORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Swimming);

    extern const TMap<uint8, FGameplayTag> MovementModeTags;
}