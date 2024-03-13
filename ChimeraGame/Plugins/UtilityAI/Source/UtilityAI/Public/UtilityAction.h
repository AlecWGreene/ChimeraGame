// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTaskOwnerInterface.h"

#include "UtilityAction.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class UTILITYAI_API UUtilityAction : public UObject, public IGameplayTaskOwnerInterface
{
	GENERATED_BODY()

	//----- Overrides -----//
public:

	UUtilityAction();

	virtual class UGameplayTasksComponent* GetGameplayTasksComponent(const class UGameplayTask& Task) const override;
	virtual AActor* GetGameplayTaskOwner(const class UGameplayTask* Task) const override;
	virtual AActor* GetGameplayTaskAvatar(const class UGameplayTask* Task) const override;
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) override;
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;

	//----- Feature API -----//
public:
	
	UFUNCTION(BlueprintNativeEvent)
	void Initialize(class UUtilityAIComponent* UtilityComponent);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float ComputeDesire(float DeltaTime, float PreviousDesire) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanActivate() const;

	UFUNCTION(BlueprintNativeEvent)
	void Activate();

	UFUNCTION(BlueprintNativeEvent)
	void Tick(float DeltaTime);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void End(bool bCancelled);

protected:

	UFUNCTION(BlueprintPure)
	bool IsActive() const { return bActive; }

	UFUNCTION(BlueprintPure)
	class UUtilityAIComponent* GetUtilityComponent() const;

	UFUNCTION(BlueprintPure)
	class UBlackboardComponent* GetBlackboardComponent() const;

	UFUNCTION(BlueprintPure)
	class AAIController* GetController() const;

	UFUNCTION(BlueprintPure)
	APawn* GetAvatar() const;

	//----- Instance Variables -----//
protected:

	bool bActive{ false };

	// agreene 2023/12/13 - #ToDo #UtilityAI compare this way of storing data vs the GAS way of 
	// an "actor info" struct
	TWeakObjectPtr<class UUtilityAIComponent> CachedUtilityComponent;
	TWeakObjectPtr<class AAIController> CachedController;
	TWeakObjectPtr<APawn> Avatar;

private:

	UPROPERTY()
	TSet<TObjectPtr<UGameplayTask>> ActiveTasks;

	// Make the UtilityAIComponent a friend so that we can debug read the data without making it public.
	friend class UUtilityAIComponent;
};