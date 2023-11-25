#include "ChimeraGameTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

namespace ChimeraTags
{
    UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "Input.Generic.Move"/*, "Input tag for moving the pawn." */);
    UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "Input.Generic.Look"/*, "Input tag for rotating the controller."*/);
}