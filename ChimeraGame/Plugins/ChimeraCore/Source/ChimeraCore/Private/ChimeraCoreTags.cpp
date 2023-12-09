// Copyright Alec Greene. All Rights Reserved.

#include "ChimeraCoreTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

namespace ChimeraCoreTags
{
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag, "Input", "Parent tag for all input tags.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "Input.Movement.Move", "Input tag for moving the pawn.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look, "Input.Movement.Look", "Input tag for rotating the controller.");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Movement_Walking,      "State.Movement.Walking",       "Movement mode for Walking.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Movement_NavWalking,   "State.Movement.NavWalking",    "Movement mode for NavWalking.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Movement_Falling,      "State.Movement.Falling",       "Movement mode for Falling.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Movement_Flying,       "State.Movement.Flying",        "Movement mode for Flying.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Movement_Swimming,     "State.Movement.Swimming",      "Movement mode for Swimming.");

    const TMap<uint8, FGameplayTag> MovementModeTags = {
        { MOVE_Walking, State_Movement_Walking },
        { MOVE_NavWalking, State_Movement_NavWalking },
        { MOVE_Falling, State_Movement_Falling },
        { MOVE_Flying, State_Movement_Flying },
        { MOVE_Swimming, State_Movement_Swimming }
    };
}