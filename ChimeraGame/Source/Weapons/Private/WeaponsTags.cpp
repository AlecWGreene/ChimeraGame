// Copyright Alec Greene. All Rights Reserved.

#include "WeaponsTags.h"

namespace WeaponsTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Hit,						"Event.Weapons.Hit",						"Event sent to the asc when a weapon hits a target.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Slot,							"Weapons.Slot",								"A slot which can hold a weapon.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Slot_Arm_Left,					"Weapons.Slot.Hand.Left",					"Left arm of a character, usually used for shields.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Slot_Arm_Right,					"Weapons.Slot.Hand.Right",					"Right arm of a character, usually used for shields.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Slot_Hand_Left,					"Weapons.Slot.Hand.Left",					"Left hand of a character, usually used for weapons.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Slot_Hand_Right,					"Weapons.Slot.Hand.Right",					"Right hand of a character, usually used for weapon.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Attack,					"State.Weapons.Attack",						"Indicates that active weapon slots want to attack. Usually used to turn on/off collision events.");
}