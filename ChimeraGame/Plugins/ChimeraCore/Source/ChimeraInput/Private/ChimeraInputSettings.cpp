// Copyright Alec Greene. All Rights Reserved.
#include "ChimeraInputSettings.h"

#include "ChimeraInputConfig.h"

const UInputAction* UChimeraInputSettings::GetAction(FGameplayTag InputTag)
{
	const UChimeraInputSettings* Settings = GetDefault<UChimeraInputSettings>();
	if (Settings->IsLoaded())
	{
		if (const TSoftObjectPtr<const UInputAction>* InputAction = Settings->InputActionMapping.Find(InputTag))
		{
			return InputAction->Get();
		}
	}

	return nullptr;
}

void UChimeraInputSettings::RequestLoadAssets()
{
	TArray<FSoftObjectPath> AssetToLoad;

	typedef TArray<TSoftObjectPtr<const UInputAction>> TInputActionArray;
	TInputActionArray InputActions;
	
	TInputActionArray MappingValues;
	InputActionMapping.GenerateValueArray(MappingValues);
	InputActions.Append(MappingValues);

	for (TSoftObjectPtr<const UInputAction> Action : InputActions)
	{
		AssetToLoad.Add(Action.ToSoftObjectPath());
	}

	LoadHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(AssetToLoad,
		FStreamableDelegate::CreateUObject(this, &UChimeraInputSettings::HandleAssetsLoaded));
}

void UChimeraInputSettings::HandleAssetsLoaded()
{
	if (LoadHandle.IsValid())
	{
		LoadHandle->GetLoadedAssets(LoadedObjects);
		bIsLoaded = true;

		for (FStreamableDelegate& StreamableDelegate : LoadDelegates)
		{
			StreamableDelegate.Execute();
		}

		LoadDelegates.Reset();
		LoadHandle.Reset();
	}
}

void UChimeraInputSettings::EnsureLoaded(const FStreamableDelegate& LoadDelegate)
{
	if (bIsLoaded)
	{
		LoadDelegate.Execute();
		return;
	}

	LoadDelegates.Add(LoadDelegate);
	if (!LoadHandle.IsValid())
	{
		RequestLoadAssets();
	}
}
