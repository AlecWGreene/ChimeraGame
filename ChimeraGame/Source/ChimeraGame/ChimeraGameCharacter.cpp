// Copyright Alec Greene. All Rights Reserved.

#include "ChimeraGameCharacter.h"

#include "ChimeraAnimSet.h"

AChimeraGameCharacter::AChimeraGameCharacter()
{

}

const UChimeraAnimSet* AChimeraGameCharacter::GetAnimSetForMesh(const USkeletalMeshComponent* InMesh) const
{
	if (InMesh == GetMesh())
	{
		return AnimSet;
	}

	return nullptr;
}