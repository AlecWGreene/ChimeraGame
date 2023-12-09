// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "AbilitySystemBlueprintLibrary.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayTagContainer.h"

#include "AnimNotify_GameplayEvent.generated.h"

UCLASS()
class CHIMERAGAS_API UAnimNotify_GameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayEventData EventData;
};