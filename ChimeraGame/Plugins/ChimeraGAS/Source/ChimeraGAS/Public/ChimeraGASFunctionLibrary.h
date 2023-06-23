#pragma once

#include "AbilitySystemBlueprintLibrary.h"
#include "ChimeraGASFunctionLibrary.generated.h"

class AActor;
class UChimeraAbilitySystemComponent;

UCLASS()
class UChimeraGASFunctionLibrary :
    public UAbilitySystemBlueprintLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
    static UChimeraAbilitySystemComponent* GetASCFromActor(AActor* Actor);

    template<class T>
    static T* GetASCFromActor(AActor* Actor);
};

template<class T>
inline T* UChimeraGASFunctionLibrary::GetASCFromActor(AActor* Actor)
{
    UAbilitySystemComponent* ASC = Super::GetAbilitySystemComponent(Actor);
    return Cast<T>(ASC);
}
