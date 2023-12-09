// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "UtilityFactors.generated.h"

USTRUCT(BlueprintType)
struct UTILITYAI_API FUtilityFactors
{
	GENERATED_BODY()

	//----- References -----/
public:

	FGameplayTagContainer GetReferenceKeys() const;

	bool GetReference(FGameplayTag Key, const UObject*& OutValue) const;
	bool HasReference(FGameplayTag Key) const;
	bool SetReference(FGameplayTag Key, const UObject* InValue);
	void SetOrAddReference(FGameplayTag Key, const UObject* InValue);

	//----- Facts -----/
public:

	FGameplayTagContainer GetFactKeys() const;

	bool GetFact(FGameplayTag Key, FVector& OutValue) const;
	bool HasFact(FGameplayTag Key) const;
	bool SetFact(FGameplayTag Key, const FVector& InValue);
	void SetOrAddFact(FGameplayTag Key, const FVector& InValue);

	bool GetFactAsBool(FGameplayTag Key, bool& OutValue) const;
	bool GetFactAsInt(FGameplayTag Key, int32& OutValue) const;
	bool GetFactAsFloat(FGameplayTag Key, float& OutValue) const;
	bool GetFactAsVector2D(FGameplayTag Key, FVector2D& OutValue) const;

protected:

	UPROPERTY()
	TMap<FGameplayTag, const UObject*> References;

	TMap<FGameplayTag, FVector> Facts;
};

UCLASS(Abstract, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class UTILITYAI_API UUtilityFactCalculator : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	FVector CalculateValue(float DeltaTime, const FVector& PreviousValue, const FUtilityFactors& Factors) const;
};