// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "ChimeraAttributeSet.h"

#include "WeaponData.generated.h"

UCLASS(BlueprintType)
class WEAPONS_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	// agreene 2023/12/5 - #ToDo #Weapons Add fields for static mesh, maybe even proper equip/unequip?
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UGameplayEffect> HitEffect;
};