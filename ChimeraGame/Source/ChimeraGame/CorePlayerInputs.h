#pragma once

#include "Engine/DataAsset.h"

#include "CorePlayerInputs.generated.h"

UCLASS()
class UCorePlayerInputs : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<class UInputAction> MoveForward;

	/** Positive values map to the right direction. */
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<class UInputAction> MoveStrafe;

	/** Positive values map to clockwise turning. */
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<class UInputAction> LookHorizontal;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<class UInputAction> LookVertical;
};