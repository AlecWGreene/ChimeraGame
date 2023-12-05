// Copyright Alec Greene. All Rights Reserved.

#include "WeaponsTags.h"

namespace WeaponsTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Slot,					"Weapons.Slot",						"A slot which can hold a weapon.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Slot_Left,				"Weapons.Slot.Left",				"Left hand of a character.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Slot_Right,				"Weapons.Slot.Right",				"Right hand of a character.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Attack,			"State.Weapons.Attack",				"Indicates that active weapon slots want to attack. Usually used to turn on/off collision events.");
}