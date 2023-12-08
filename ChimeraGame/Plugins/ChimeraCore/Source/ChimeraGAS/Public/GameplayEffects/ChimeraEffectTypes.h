// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbilityTargetDataFilter.h"
#include "GameplayEffect.h"	
#include "GenericTeamAgentInterface.h"

#include "ChimeraEffectTypes.generated.h"

// agreene 2023/12/5 - #ToDo #GAS Look to implement a way to composite effects from a list of components
USTRUCT(BlueprintType)
struct CHIMERAGAS_API FGameplayEffectSpecDef
{
	GENERATED_BODY()

public:

	FGameplayEffectSpecDef() {}
	
	FGameplayEffectSpecDef(
		TSubclassOf<UGameplayEffect> InEffectClass,
		int32 InLevel = 1,
		const FGameplayTagContainer& InDynamicAssetTags = FGameplayTagContainer(),
		const FGameplayTagContainer& InDynamicGrantedTags = FGameplayTagContainer(),
		const TMap<FGameplayTag, float>& InSetByCallers = TMap<FGameplayTag, float>()) 
	{
		EffectClass = InEffectClass;
		Level = InLevel;
		DynamicAssetTags = InDynamicAssetTags;
		DynamicGrantedTags = InDynamicGrantedTags;
		SetByCallers = InSetByCallers;
	}

	FGameplayEffectSpecHandle CreateSpec(const class UAbilitySystemComponent* ASC, FGameplayEffectContextHandle Context = FGameplayEffectContextHandle()) const;

	/** Applies these settings to the spec handle. */
	bool ModifySpec(FGameplayEffectSpecHandle& InOutSpec) const;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	int32 Level{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	FGameplayTagContainer DynamicAssetTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	FGameplayTagContainer DynamicGrantedTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "SetByCaller", ForceInlineRow))
	TMap<FGameplayTag, float> SetByCallers;
};

USTRUCT(BlueprintType)
struct CHIMERAGAS_API FChimeraTargetDataFilter : public FGameplayTargetDataFilter
{
	GENERATED_BODY()

public:
	virtual bool FilterPassesForActor(const AActor* Actor) const;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagQuery TargetTagConditions;

	/** When true, the filter will allow SelfActor to be targeted even if SelfActor doesn't pass the team attitude check. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "SelfFilter!=ETargetDataFilterSelf::TDFS_NoSelf"))
	bool bOverrideTeamAttitudeForSelf{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BitMask, BitMaskEnum = "/Script/AIModule.ETeamAttitude"))
	uint8 TargetAttitudes{ ETeamAttitude::Hostile };
};