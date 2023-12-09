// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "GameplayTagContainer.h"

#include "AnimNotifyState_ActivateWeapon.generated.h"

UCLASS()
class WEAPONS_API UAnimNotifyState_ActivateWeapon : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp, 
		UAnimSequenceBase* Animation, 
		float TotalDuration, 
		const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp, 
		UAnimSequenceBase* Animation, 
		const FAnimNotifyEventReference& EventReference) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Weapons.Slot"))
	FGameplayTagContainer WeaponSlots;

public:

	TWeakObjectPtr<class UWeaponsComponent> WeaponsComponent;
};