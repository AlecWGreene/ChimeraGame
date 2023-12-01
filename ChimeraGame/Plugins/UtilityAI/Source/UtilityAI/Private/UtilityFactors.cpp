// Copyright Alec Greene. All Rights Reserved.
#include "UtilityFactors.h"

FGameplayTagContainer FUtilityFactors::GetReferenceKeys() const
{
    TArray<FGameplayTag> KeyArray;
    References.GenerateKeyArray(KeyArray);

    return FGameplayTagContainer::CreateFromArray(KeyArray);
}

bool FUtilityFactors::GetReference(FGameplayTag Key, const UObject* OutValue) const
{
    if (const UObject* const* RefValue = References.Find(Key))
    {
        OutValue = *RefValue;
        return true;
    }

    return false;
}

bool FUtilityFactors::HasReference(FGameplayTag Key) const
{
    return References.Contains(Key);
}

bool FUtilityFactors::SetReference(FGameplayTag Key, const UObject* InValue)
{
    if (const UObject** RefValue = References.Find(Key))
    {
        *RefValue = InValue;
        return true;
    }

    return false;
}

void FUtilityFactors::SetOrAddReference(FGameplayTag Key, const UObject* InValue)
{
    const UObject*& FactValue = References.FindOrAdd(Key);
    FactValue = InValue;
}

FGameplayTagContainer FUtilityFactors::GetFactKeys() const
{
    TArray<FGameplayTag> KeyArray;
    Facts.GenerateKeyArray(KeyArray);

    return FGameplayTagContainer::CreateFromArray(KeyArray);
}

bool FUtilityFactors::GetFact(FGameplayTag Key, FVector& OutValue) const
{
    if (const FVector* FactValue = Facts.Find(Key))
    {
        OutValue = *FactValue;
        return true;
    }

    return false;
}

bool FUtilityFactors::HasFact(FGameplayTag Key) const
{
    return Facts.Contains(Key);
}

bool FUtilityFactors::SetFact(FGameplayTag Key, const FVector& InValue)
{
    if (FVector* FactValue = Facts.Find(Key))
    {
        *FactValue = InValue;
        return true;
    }

    return false;
}

void FUtilityFactors::SetOrAddFact(FGameplayTag Key, const FVector& InValue)
{
    FVector& FactValue = Facts.FindOrAdd(Key);
    FactValue = InValue;
}

bool FUtilityFactors::GetFactAsBool(FGameplayTag Key, bool& OutValue) const
{
    if (const FVector* FactValue = Facts.Find(Key))
    {
        OutValue = static_cast<bool>(FactValue->X);
        return true;
    }

    return false;
}

bool FUtilityFactors::GetFactAsInt(FGameplayTag Key, int32& OutValue) const
{
    if (const FVector* FactValue = Facts.Find(Key))
    {
        OutValue = static_cast<int32>(FactValue->X);
        return true;
    }

    return false;
}

bool FUtilityFactors::GetFactAsFloat(FGameplayTag Key, float& OutValue) const
{
    if (const FVector* FactValue = Facts.Find(Key))
    {
        OutValue = FactValue->X;
        return true;
    }

    return false;
}

bool FUtilityFactors::GetFactAsVector2D(FGameplayTag Key, FVector2D& OutValue) const
{
    if (const FVector* FactValue = Facts.Find(Key))
    {
        OutValue = FVector2D(FactValue->X, FactValue->Y);
        return true;
    }

    return false;
}

FVector UUtilityFactCalculator::CalculateValue_Implementation(float DeltaTime, const FVector& PreviousValue, const FUtilityFactors& Factors) const
{
    return FVector::ZeroVector;
}
