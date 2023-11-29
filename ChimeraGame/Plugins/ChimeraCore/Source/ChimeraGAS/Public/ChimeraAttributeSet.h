// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "AttributeSet.h"

#include "ChimeraAbilitySystemComponent.h"

#include "ChimeraAttributeSet.generated.h"

// Consolidate macros from the GameplayAbilities module for ease of use
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class CHIMERAGAS_API UChimeraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
};

UCLASS(BlueprintType, DefaultToInstanced)
class CHIMERAGAS_API UChimeraAttributeSetInitializer : public UObject
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UChimeraAttributeSet> AttributeSetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ReadOnlyKeys, ForceInlineRow))
	TMap<FGameplayAttribute, float> AttributeDefaults;
};