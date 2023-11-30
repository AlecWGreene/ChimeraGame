// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "GameplayEffect.h"

#include "ChimeraEffectTypes.generated.h"

USTRUCT(BlueprintType)
struct CHIMERAGAS_API FGameplayEffectSpecDef
{
	GENERATED_BODY()

public:

	FGameplayEffectSpecDef() {}
	
	FGameplayEffectSpecDef(
		TSubclassOf<UGameplayEffect> InEffectClass,
		int32 InLevel = 1,
		float InChanceToApply = 1.f,
		const FGameplayTagContainer& InDynamicAssetTags = FGameplayTagContainer(),
		const FGameplayTagContainer& InDynamicGrantedTags = FGameplayTagContainer(),
		const TMap<FGameplayTag, float>& InSetByCallers = TMap<FGameplayTag, float>(),
		const TArray<FConditionalGameplayEffect>& InConditionalEffects = TArray<FConditionalGameplayEffect>(),
		const TArray<FGameplayAbilitySpecDef>& InGrantedAbilities = TArray<FGameplayAbilitySpecDef>()) 
	{
		EffectClass = InEffectClass;
		Level = InLevel;
		ChanceToApply = InChanceToApply;
		DynamicAssetTags = InDynamicAssetTags;
		DynamicGrantedTags = InDynamicGrantedTags;
		SetByCallers = InSetByCallers;
		ConditionalEffects = InConditionalEffects;
		GrantedAbilities = InGrantedAbilities;
	}

	FGameplayEffectSpecHandle CreateSpec(const class UAbilitySystemComponent* ASC, FGameplayEffectContextHandle Context = FGameplayEffectContextHandle()) const;

	/** Applies these settings to the spec handle. */
	bool ModifySpec(FGameplayEffectSpecHandle& InOutSpec) const;

protected:

	TArray<FGameplayEffectSpecHandle> CreateLinkedEffectsList(FGameplayEffectContextHandle Context, FGameplayTagContainer CapturedSourceTags) const;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	int32 Level{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	float ChanceToApply{ 1.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	FGameplayTagContainer DynamicAssetTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	FGameplayTagContainer DynamicGrantedTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "SetByCaller", ForceInlineRow))
	TMap<FGameplayTag, float> SetByCallers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	TArray<FConditionalGameplayEffect> ConditionalEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	TArray<FGameplayAbilitySpecDef> GrantedAbilities;
};