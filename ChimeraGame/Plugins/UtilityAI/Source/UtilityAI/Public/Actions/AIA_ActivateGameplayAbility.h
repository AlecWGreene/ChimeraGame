// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UtilityAction.h"

#include "GameplayTagContainer.h"

#include "AIA_ActivateGameplayAbility.generated.h"

UCLASS()
class UTILITYAI_API UAIA_ActivateGameplayAbility : public UUtilityAction
{
	GENERATED_BODY()

	//----- Overrides -----//
public:

	virtual bool CanActivate_Implementation() const override;
	virtual void Activate_Implementation() override;
	virtual void End_Implementation(bool bCancelled) override;

	//----- Utility Methods -----//
protected:

	TArray<struct FGameplayAbilitySpec*> FindAbilitySpecs() const;

	UFUNCTION()
	void HandleAbilityEnded(class UGameplayAbility* AbilityInstance);

	//----- Class Settings -----//
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWaitForAbilityEnd{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseTagQuery{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "AbilityTagCategory", EditCondition = "bUseTagQuery", EditConditionHides))
	FGameplayTagQuery AbilityTagQuery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bUseTagQuery", EditConditionHides))
	TSubclassOf<class UGameplayAbility> AbilityClass;

	//----- Instance Variables -----//
protected:
};