// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerController.h"

#include "ChimeraPlayerController.generated.h"

UCLASS(Blueprintable, BlueprintType)
class CHIMERACORE_API AChimeraPlayerController :
    public APlayerController
{
    GENERATED_BODY()

    //----- APlayerController Overrides -----//
public:

    //----- Class Settings -----//
public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TArray<TObjectPtr<const class UInputMappingContext>> DefaultInputMappingContexts;
};

