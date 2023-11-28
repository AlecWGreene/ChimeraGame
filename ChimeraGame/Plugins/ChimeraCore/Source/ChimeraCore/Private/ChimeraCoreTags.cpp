// Copyright Alec Greene. All Rights Reserved.

#include "ChimeraCoreTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

namespace ChimeraCoreTags
{
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag, "Input", "Parent tag for all input tags.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "Input.Movement.Move", "Input tag for moving the pawn.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look, "Input.Movement.Look", "Input tag for rotating the controller.");
}