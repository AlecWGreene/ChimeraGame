// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "ChimeraAnimSet.generated.h"

/**
 * Holds the animations for a character or skeletal mesh.
 */
UCLASS(BlueprintType)
class CHIMERACORE_API UChimeraAnimSet : public UDataAsset
{
	GENERATED_BODY()

public:

	/** Montages used by this anim set. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ForceInlineRow, Categories = "Anim.Montage"))
	TMap<FGameplayTag, TObjectPtr<const UAnimMontage>> Montages;
};