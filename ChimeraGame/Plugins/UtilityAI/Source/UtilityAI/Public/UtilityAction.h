// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "UtilityFactors.h"

#include "UtilityAction.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class UTILITYAI_API UUtilityAction : public UObject
{
	GENERATED_BODY()

public:

	UUtilityAction();

public:
	
	UFUNCTION(BlueprintNativeEvent)
	float ComputeDesire(float DeltaTime, float PreviousDesire, const FUtilityFactors& Factors) const;
	
	UFUNCTION(BlueprintNativeEvent)
	bool CanActivate(const FUtilityFactors& Factors) const;
	
	UFUNCTION(BlueprintNativeEvent)
	void Activate(const FUtilityFactors& Factors);
	
	UFUNCTION(BlueprintNativeEvent)
	void Tick(float DeltaTime);
	
	UFUNCTION(BlueprintNativeEvent)
	void End();

protected:

	TWeakObjectPtr<class UUtilityAIComponent> OwningComponent;

	FUtilityFactors InitialFactors;

	// Make the UtilityAIComponent a friend so that we can initialize the owning component without making it public.
	friend class UUtilityAIComponent;
};