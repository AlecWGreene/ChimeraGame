// Copyright Alec Greene. All Rights Reserved.

#include "ChimeraGameTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

namespace ChimeraGameTags
{
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(AnimTag_Walk_Fwd, "Anim.Walk.Fwd", "Animation for walking forward.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(AnimTag_Jog_Fwd, "Anim.Walk.Fwd", "Animation for walking forward.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(AnimTag_Look, "Input.Generic.Look", "Input tag for rotating the controller.");
}