// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "UtilityAction.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class UTILITYAI_API UUtilityAction : public UObject
{
	GENERATED_BODY()

public:

	UUtilityAction();

public:
	
	UFUNCTION(BlueprintNativeEvent)
	float ComputeDesire(float DeltaTime, float PreviousDesire, const class UBlackboardComponent* BBComponent) const;
	
	UFUNCTION(BlueprintNativeEvent)
	bool CanActivate(const class UBlackboardComponent* BBComponent) const;
	
	UFUNCTION(BlueprintNativeEvent)
	void Activate(class UBlackboardComponent* BBComponent);
	
	UFUNCTION(BlueprintNativeEvent)
	void Tick(float DeltaTime);
	
	UFUNCTION(BlueprintNativeEvent)
	void End();

protected:

	TWeakObjectPtr<class UUtilityAIComponent> OwningComponent;

	// Make the UtilityAIComponent a friend so that we can initialize the owning component without making it public.
	friend class UUtilityAIComponent;
};