#pragma once

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"

#include "GameplayEffects/ChimeraEffectTypes.h"

#include "ChimeraGASFunctionLibrary.generated.h"

class AActor;
class UChimeraAbilitySystemComponent;

UCLASS()
class CHIMERAGAS_API UChimeraGASFunctionLibrary :
    public UAbilitySystemBlueprintLibrary
{
    GENERATED_BODY()

    //----- ASC -----//
public:

    UFUNCTION(BlueprintCallable)
    static UChimeraAbilitySystemComponent* GetChimeraASC(const AActor* Actor);

    template<class T>
    static T* GetASCFromActor(const AActor* Actor);

    //----- GameplayEffects -----//
public:

    UFUNCTION(BlueprintCallable)
    static FGameplayEffectSpecHandle MakeSpecFromDef(
        const UAbilitySystemComponent* ASC, 
        FGameplayEffectContextHandle Context, 
        const FGameplayEffectSpecDef& SpecDef);

    //----- Target Data -----//
public:

    UFUNCTION(BlueprintCallable)
    static bool TargetDataFilterPassesForActor(const AActor* Actor, const FChimeraTargetDataFilter& Filter);
};

template<class T>
inline T* UChimeraGASFunctionLibrary::GetASCFromActor(const AActor* Actor)
{
    UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
    return Cast<T>(ASC);
}
