#pragma once

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "ChimeraGASFunctionLibrary.generated.h"

class AActor;
class UChimeraAbilitySystemComponent;

UCLASS()
class CHIMERAGAS_API UChimeraGASFunctionLibrary :
    public UAbilitySystemBlueprintLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
    static UChimeraAbilitySystemComponent* GetChimeraASC(const AActor* Actor);

    template<class T>
    static T* GetASCFromActor(const AActor* Actor);
};

template<class T>
inline T* UChimeraGASFunctionLibrary::GetASCFromActor(const AActor* Actor)
{
    UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
    return Cast<T>(ASC);
}
