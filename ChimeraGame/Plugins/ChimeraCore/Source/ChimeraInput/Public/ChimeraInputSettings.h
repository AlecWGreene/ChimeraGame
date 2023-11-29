// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "Engine/AssetManager.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/StreamableManager.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"

#include "ChimeraInputSettings.generated.h"

UCLASS(config = Input, defaultconfig)
class CHIMERAINPUT_API UChimeraInputSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	//----- Settings -----//
public:

	// agreene 2023/11/28 - #ToDo #Input Set this up to optionally be auto-populated through setting a parent tag
	UPROPERTY(EditAnywhere, BlueprintReadOnly, config, meta = (ForceInlineRow))
	TMap<FGameplayTag, TSoftObjectPtr<const class UInputAction>> InputActionMapping;

	//----- Utility Methods -----//
public:

	UFUNCTION(BlueprintCallable, meta = (Categories = "Input"))
	static const UInputAction* GetAction(FGameplayTag InputTag);

	//----- Loading -----//
public:

	bool IsLoaded() const { return bIsLoaded; }
	void EnsureLoaded(const FStreamableDelegate& LoadDelegate);

protected:
	void RequestLoadAssets();
	void HandleAssetsLoaded();

	bool bIsLoaded{ false };

	TSharedPtr<FStreamableHandle> LoadHandle;
	TArray<FStreamableDelegate> LoadDelegates;

	/** 
	 * Many assets like input actions/configs are light enough we want to keep them permanently loaded
	 * so we force this to happen by storing them here
	 */
	UPROPERTY()
	TArray<UObject*> LoadedObjects;
};