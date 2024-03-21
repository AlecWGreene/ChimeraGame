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

	//----- Feature Methods -----//
public:

	UFUNCTION(BlueprintCallable)
	void UpdateActionSet(class UUtilityActionSet* InActionSet);

protected:

	virtual void UpdateDesires(float DeltaTime);
	virtual void SelectNewAction();

	//----- Debug -----//
protected:

	virtual FString GetDebugInfoString() const override;

	//----- Class Settings -----//
protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UUtilityActionSet> ActionSet;

	//----- Instance Variables -----//
protected:

	bool bLogicRunning{ true };

	UPROPERTY()
	TObjectPtr<class UBlackboardData> Blackboard;

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<class UUtilityAction>> AvailableActions;

	/** Actions which are requested to be activated regardless of utility. */
	FGameplayTagContainer ActionOverrides;

	/** Cached desires for each action. */
	TMap<FGameplayTag, float> ActionDesires;

	UPROPERTY()
	class UUtilityAction* ActiveAction{ nullptr };
};