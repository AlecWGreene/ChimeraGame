// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "UtilityFactors.h"

#include "UtilityAIFunctionLibrary.generated.h"

UCLASS()
class UTILITYAI_API UUtilityAIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//----- Factors -----//
public:

	UFUNCTION(BlueprintCallable, Category = "Utility AI|Factors")
	static bool GetReference(const FUtilityFactors& UtilityFactors, FGameplayTag Key, const UObject* OutValue);
	
	UFUNCTION(BlueprintCallable, Category = "Utility AI|Factors")
	static bool HasReference(const FUtilityFactors& UtilityFactors, FGameplayTag Key);
	
	UFUNCTION(BlueprintCallable, Category = "Utility AI|Factors")
	static bool GetFact(const FUtilityFactors& UtilityFactors, FGameplayTag Key, FVector& OutValue);
	
	UFUNCTION(BlueprintCallable, Category = "Utility AI|Factors")
	static bool HasFact(const FUtilityFactors& UtilityFactors, FGameplayTag Key);

	UFUNCTION(BlueprintCallable, Category = "Utility AI|Factors")
	static bool GetFactAsBool(const FUtilityFactors& UtilityFactors, FGameplayTag Key, bool& OutValue);
	
	UFUNCTION(BlueprintCallable, Category = "Utility AI|Factors")
	static bool GetFactAsInt(const FUtilityFactors& UtilityFactors, FGameplayTag Key, int32& OutValue);
	
	UFUNCTION(BlueprintCallable, Category = "Utility AI|Factors")
	static bool GetFactAsFloat(const FUtilityFactors& UtilityFactors, FGameplayTag Key, float& OutValue);
	
	UFUNCTION(BlueprintCallable, Category = "Utility AI|Factors")
	static bool GetFactAsVector2D(const FUtilityFactors& UtilityFactors, FGameplayTag Key, FVector2D& OutValue);

	UFUNCTION(BlueprintCallable, Category = "Utility AI|Factors|Conversion")
	static bool ConvertDataToBool(const FVector& InData);

	UFUNCTION(BlueprintCallable, Category = "Utility AI|Factors|Conversion")
	static int32 ConvertDataToInt(const FVector& InData);

	UFUNCTION(BlueprintCallable, Category = "Utility AI|Factors|Conversion")
	static float ConvertDataToFloat(const FVector& InData);

	UFUNCTION(BlueprintCallable, Category = "Utility AI|Factors|Conversion")
	static FVector2D ConvertDataToVector2D(const FVector& InData);
};