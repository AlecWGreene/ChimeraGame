// Copyright Alec Greene. All Rights Reserved.
#include "UtilityAIFunctionLibrary.h"

bool UUtilityAIFunctionLibrary::GetReference(const FUtilityFactors& UtilityFactors, FGameplayTag Key, const UObject* OutValue)
{
	return false;
}

bool UUtilityAIFunctionLibrary::HasReference(const FUtilityFactors& UtilityFactors, FGameplayTag Key)
{
	return false;
}

bool UUtilityAIFunctionLibrary::GetFact(const FUtilityFactors& UtilityFactors, FGameplayTag Key, FVector& OutValue)
{
	return false;
}

bool UUtilityAIFunctionLibrary::HasFact(const FUtilityFactors& UtilityFactors, FGameplayTag Key)
{
	return false;
}

bool UUtilityAIFunctionLibrary::GetFactAsBool(const FUtilityFactors& UtilityFactors, FGameplayTag Key, bool& OutValue)
{
	return false;
}

bool UUtilityAIFunctionLibrary::GetFactAsInt(const FUtilityFactors& UtilityFactors, FGameplayTag Key, int32& OutValue)
{
	return false;
}

bool UUtilityAIFunctionLibrary::GetFactAsFloat(const FUtilityFactors& UtilityFactors, FGameplayTag Key, float& OutValue)
{
	return false;
}

bool UUtilityAIFunctionLibrary::GetFactAsVector2D(const FUtilityFactors& UtilityFactors, FGameplayTag Key, FVector2D& OutValue)
{
	return false;
}

bool UUtilityAIFunctionLibrary::ConvertDataToBool(const FVector& InData)
{
	return static_cast<bool>(InData.X);
}

int32 UUtilityAIFunctionLibrary::ConvertDataToInt(const FVector& InData)
{
	return static_cast<int32>(InData.X);
}

float UUtilityAIFunctionLibrary::ConvertDataToFloat(const FVector& InData)
{
	return InData.X;
}

FVector2D UUtilityAIFunctionLibrary::ConvertDataToVector2D(const FVector& InData)
{
	return FVector2D(InData.X, InData.Y);
}
