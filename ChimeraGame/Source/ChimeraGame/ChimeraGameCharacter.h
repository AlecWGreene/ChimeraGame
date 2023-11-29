// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "Engine/StreamableManager.h"

#include "ChimeraCharacter.h"

#include "ChimeraGameCharacter.generated.h"

UCLASS(Blueprintable, BlueprintType, config = Game)
class CHIMERAGAME_API AChimeraGameCharacter : public AChimeraCharacter
{
	GENERATED_BODY()

	//----- Overrides -----//
public:
	AChimeraGameCharacter();
	virtual void BeginPlay() override;
	virtual const class UChimeraAnimSet* GetAnimSetForMesh(FGameplayTag AnimSetTag, const USkeletalMeshComponent* InMesh) const override;

	//----- Class Settings -----//
public:

	// agreene 2023/11/27 - #ToDo Move this to async loading
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Anim.Set"))
	TMap<FGameplayTag, TSoftObjectPtr<const class UChimeraAnimSet>> AnimSets;

	//----- Loading -----//
public:
	// agreene 2023/11/28 - #ToDo #Loading Setup a wrapper struct for managing async loading
	bool AreAnimsLoaded() const { return bAnimsLoaded; }
	void EnsureAnimsLoaded(const FStreamableDelegate& LoadDelegate);

protected:

	void RequestLoadAnims();
	void HandleAnimsLoaded();

	bool bAnimsLoaded{ false };

	TSharedPtr<FStreamableHandle> AnimLoadHandle;
	TArray<FStreamableDelegate> AnimLoadDelegates;
};