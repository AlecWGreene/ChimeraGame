#pragma once

#include "GameFramework/PlayerController.h"

#include "ChimeraPlayerController.generated.h"

UCLASS()
class AChimeraPlayerController :
    public APlayerController
{
    GENERATED_BODY()

    //----- APlayerController Overrides -----//
public:
    virtual void PostProcessInput(const float DeltaTime, const bool bPaused);

    //----- Class Settings -----//
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<const class UChimeraInputConfig> DefaultInputMappingConfig;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TArray<TObjectPtr<const class UInputMappingContext>> DefaultInputMappingContexts;
};

