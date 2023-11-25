#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ChimeraInputConfig.generated.h"

/** Links an input action to a GameplayTag. */
USTRUCT(BlueprintType)
struct CHIMERAINPUT_API FChimeraInputLink
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UInputAction> InputAction;

	// @agreene #Todo - 2023/06/17 - This Categories value should be a placeholder for the plugin user to specify in config
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories="Input"))
	FGameplayTag InputTag;

public:

	bool IsValid() const
	{
		return InputAction != nullptr && InputTag.IsValid();
	}
};

/**
* Used to relate input actions to GameplayTags, so that other systems may handle the input actions without directly linking to them.
* Also enables easier remapping of keys.
*/
UCLASS()
class CHIMERAINPUT_API UChimeraInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	const class UInputAction* GetNativeAction(FGameplayTag InputTag) const;

	UFUNCTION(BlueprintCallable)
	const TArray<FChimeraInputLink>& GetAbilityLinks() const;

protected:

	/** Input actions which will link to a native function. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "{InputAction} -> {InputTag}"))
	TArray<FChimeraInputLink> NativeLinks;

	/** Input actions which will link to the ASC. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "{InputAction} -> {InputTag}"))
	TArray<FChimeraInputLink> AbilityLinks;
};