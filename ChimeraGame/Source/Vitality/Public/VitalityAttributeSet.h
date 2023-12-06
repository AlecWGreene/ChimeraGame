// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "ChimeraAttributeSet.h"

#include "VitalityAttributeSet.generated.h"

UCLASS()
class VITALITY_API UVitalityAttributeSet : public UChimeraAttributeSet
{
	GENERATED_BODY()

	//----- Overrides -----//
public:

	UVitalityAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	//----- Feature Methods -----//
public:

	void HandleDamage(const FGameplayEffectSpec& Spec, float Amount);
	void HandleHealing(const FGameplayEffectSpec& Spec, float Amount);

	//----- Attributes -----//
public:

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UVitalityAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UVitalityAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos, MetaAttribute))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UVitalityAttributeSet, Damage);

	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos, MetaAttribute))
	FGameplayAttributeData Healing;
	ATTRIBUTE_ACCESSORS(UVitalityAttributeSet, Healing);
};