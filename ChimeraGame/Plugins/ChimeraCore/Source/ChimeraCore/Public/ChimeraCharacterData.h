// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "ChimeraAttributeSet.h"

#include "ChimeraCharacterData.generated.h"

// agreene 2023/11/29 - #ToDo #Character Create spawning methods which take this class and a blueprint class
UCLASS(BlueprintType)
class CHIMERACORE_API UChimeraCharacterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<const UChimeraAttributeSetInitializer>> Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGameplayAbilitySpecDef> Abilities;
};