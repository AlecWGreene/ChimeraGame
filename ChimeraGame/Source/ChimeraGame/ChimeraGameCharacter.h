// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "ChimeraCharacter.h"
#include "ChimeraGameCharacter.generated.h"

UCLASS(Blueprintable, BlueprintType, config = Game)
class CHIMERAGAME_API AChimeraGameCharacter : public AChimeraCharacter
{
	GENERATED_BODY()

	//----- Overrides -----//
public:
	AChimeraGameCharacter();

	virtual const class UChimeraAnimSet* GetAnimSetForMesh(const USkeletalMeshComponent* InMesh) const override;

	//----- Class Settings -----//
public:

	// agreene 2023/11/27 - #ToDo Move this to async loading
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<const class UChimeraAnimSet> AnimSet;
};