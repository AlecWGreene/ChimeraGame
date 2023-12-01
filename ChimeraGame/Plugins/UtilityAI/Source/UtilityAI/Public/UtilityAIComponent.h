// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "UtilityAIComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnable))
class UTILITYAI_API UUtilityAIComponent : public UActorComponent
{
	GENERATED_BODY()

	//----- Overrides -----//
public:

	UUtilityAIComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//----- Lifecycle -----//
protected:

	virtual void UpdateFacts(float DeltaTime);
	virtual void UpdateDesires(float DeltaTime);

	//----- Factors -----//
public:

	UFUNCTION(BlueprintPure)
	const FUtilityFactors& GetFactors() const { return Factors; }

	UFUNCTION(BlueprintCallable)
	void UpdateReference(FGameplayTag ReferenceKey, const UObject* Object);

	//----- Debug -----//
protected:

	static void OnDisplayDebug(class AHUD* HUD, class UCanvas* Canvas, const class FDebugDisplayInfo& DebugDisplay, float YL, float YPos);
	virtual void DisplayDebug(class AHUD* HUD, class UCanvas* Canvas, const class FDebugDisplayInfo& DebugDisplay, float YL, float YPos);

	//----- Class Settings -----//
protected:

	// agreene 2023/11/30 - #ToDo #UtilityAI Allow add/removal of actions
	UPROPERTY(EditDefaultsOnly, Instanced, meta = (ForceInlineRow))
	TMap<FGameplayTag, TObjectPtr<class UUtilityAction>> Actions;

	// agreene 2023/11/30 - #ToDo #UtilityAI Allow add/removal of calculators
	// agreene 2023/11/30 - #ToDo #UtilityAI Setup fact dependencies and data validation
	UPROPERTY(EditDefaultsOnly, Instanced, meta = (ForceInlineRow))
	TMap<FGameplayTag, TObjectPtr<class UUtilityFactCalculator>> FactCalculators;

	//----- Instance Variables -----//
protected:
	FUtilityFactors Factors;

	/** Cached off action desires, which allows us to look up desires without having to ask the UtilityAction. */
	TMap<FGameplayTag, float> ActionDesires;
};