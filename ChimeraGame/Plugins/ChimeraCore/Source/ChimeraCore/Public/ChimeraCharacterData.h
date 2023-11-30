// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "ChimeraAttributeSet.h"

#include "ChimeraCharacterData.generated.h"

UCLASS(BlueprintType)
class CHIMERACORE_API UChimeraCharacterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<const UChimeraAttributeSetInitializer>> Attributes;
};