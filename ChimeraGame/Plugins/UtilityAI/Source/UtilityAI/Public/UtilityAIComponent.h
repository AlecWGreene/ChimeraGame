// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "BrainComponent.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "UtilityFactors.h"

#include "UtilityAIComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class UTILITYAI_API UUtilityAIComponent : public UBrainComponent
{
	GENERATED_BODY()

	friend class FUtilityAIModule;

	//----- Overrides -----//
public:

	UUtilityAIComponent();

	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool IsRunning() const override;
	virtual bool IsPaused() const override;

	virtual void StartLogic() override;
	virtual void PauseLogic(const FString& Reason) override;
	virtual void RestartLogic() override;
	virtual void StopLogic(const FString& Reason) override;
	virtual void Cleanup() override;

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
	static void OnDisplayDebug(class AHUD* HUD, class UCanvas* Canvas, const class FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos);
	virtual void DisplayDebug(class AHUD* HUD, class UCanvas* Canvas, const class FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos);

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