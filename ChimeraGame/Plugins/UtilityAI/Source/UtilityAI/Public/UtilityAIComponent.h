// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "BrainComponent.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

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

	virtual void UpdateDesires(float DeltaTime);

	//----- Debug -----//
protected:

	virtual FString GetDebugInfoString() const override;

	//----- Class Settings -----//
protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UBlackboardData> Blackboard;

	// agreene 2024/03/18 - #ToDo #UtilityAI Plan out inheritance of actions, and where data should live
	// agreene 2023/11/30 - #ToDo #UtilityAI Allow add/removal of actions
	UPROPERTY(EditDefaultsOnly, Instanced, meta = (ForceInlineRow, Categories = ""))
	TMap<FGameplayTag, TObjectPtr<class UUtilityAction>> Actions;

	UPROPERTY(EditDefaultsOnly, Instanced, meta = (ForceInlineRow, Categories = ""))
	TMap<FGameplayTag, TObjectPtr<class UObject>> DecisionFactorCalculations;

	//----- Instance Variables -----//
protected:

	class UUtilityAction* ActiveAction{ nullptr };

	TMap<FGameplayTag, float> DecisionFactors;

	/** Cached off action desires, which allows us to look up desires without having to ask the UtilityAction. */
	TMap<FGameplayTag, float> ActionDesires;
};