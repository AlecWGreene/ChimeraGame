// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "ChimeraAnimSet.generated.h"

/**
 * Holds the start/loop/stop animations for an animation state.
 */
USTRUCT(BlueprintType)
struct CHIMERACORE_API FChimeraStateAnims
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<const UAnimSequence> Start;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<const UAnimSequence> Loop;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<const UAnimSequence> Stop;
};

/**
 * Holds the animations for a character or skeletal mesh.
 */
UCLASS(BlueprintType)
class CHIMERACORE_API UChimeraAnimSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** Animations used for locomotion, such as walking or jumping. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ForceInlineRow, Categories = "Anim"))
	TMap<FGameplayTag, FChimeraStateAnims> LocomotionAnimationStates;

	/** Montages used by this anim set. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ForceInlineRow, Categories = "Anim.Montage"))
	TMap<FGameplayTag, TObjectPtr<const UAnimMontage>> Montages;
};