// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "UtilityActionSet.generated.h"

UCLASS()
class UTILITYAI_API UUtilityActionSet : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Instanced, meta = (ForceInlineRow, Categories = ""))
	TMap<FGameplayTag, TObjectPtr<class UUtilityAction>> Actions;
};