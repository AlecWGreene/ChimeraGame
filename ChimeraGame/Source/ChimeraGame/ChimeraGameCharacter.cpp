// Copyright Alec Greene. All Rights Reserved.

#include "ChimeraGameCharacter.h"

#include "Engine/AssetManager.h"

#include "ChimeraAnimSet.h"
#include "WeaponsComponent.h"

AChimeraGameCharacter::AChimeraGameCharacter()
{

}

void AChimeraGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	RequestLoadAnims();

	// agreene 2023/12/6 - #ToDo #Weapons maybe its time to add the component to the c++ class?
	if (UWeaponsComponent* WeaponsComponent = FindComponentByClass<UWeaponsComponent>())
	{
		for (TPair<FGameplayTag, TObjectPtr<const UWeaponData>> SlotData : DefaultWeapons)
		{
			WeaponsComponent->GiveWeapon(SlotData.Key, SlotData.Value);
		}
	}
}

const UChimeraAnimSet* AChimeraGameCharacter::GetAnimSetForMesh(FGameplayTag AnimSetTag, const USkeletalMeshComponent* InMesh) const
{
	if (InMesh == GetMesh())
	{
		if (const TSoftObjectPtr<const UChimeraAnimSet>* AnimSetPtr = AnimSets.Find(AnimSetTag))
		{
			return AnimSetPtr->Get();
		}
	}

	return nullptr;
}

void AChimeraGameCharacter::EnsureAnimsLoaded(const FStreamableDelegate& LoadDelegate)
{
	if (AreAnimsLoaded())
	{
		LoadDelegate.Execute();
	}

	AnimLoadDelegates.Add(LoadDelegate);
	if (!AnimLoadHandle.IsValid())
	{
		RequestLoadAnims();
	}
}

void AChimeraGameCharacter::RequestLoadAnims()
{
	TArray<FSoftObjectPath> AssetsToLoad;

	TArray<TSoftObjectPtr<const UChimeraAnimSet>> AnimSetList;
	AnimSets.GenerateValueArray(AnimSetList);
	for (TSoftObjectPtr<const UChimeraAnimSet> AnimSet : AnimSetList)
	{
		AssetsToLoad.Add(AnimSet.ToSoftObjectPath());
	}

	if (!AssetsToLoad.IsEmpty())
	{
		AnimLoadHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(AssetsToLoad,
			FStreamableDelegate::CreateUObject(this, &ThisClass::HandleAnimsLoaded));
	}
}

void AChimeraGameCharacter::HandleAnimsLoaded()
{
	if (AnimLoadHandle.IsValid())
	{
		for (const FStreamableDelegate& Delegate : AnimLoadDelegates)
		{
			Delegate.ExecuteIfBound();
		}

		AnimLoadDelegates.Reset();
		AnimLoadHandle.Reset();
	}
}
