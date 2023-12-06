#pragma once

#include "Abilities/GameplayAbility.h"
#include "GameplayEffects/ChimeraEffectTypes.h"

#include "ChimeraAbilitySystemComponent.h"

#include "ChimeraGameplayAbility.generated.h"

UENUM()
enum class EAbilityActivationPolicy : uint8
{
    /** Activate when GiveAbility is called. */
    OnGranted,
    /** Wait for an ability trigger to activate. */
    OnAbilityTrigger,
    /** Attempt to active each frame as long as the input is held. */
    OnInput
};

UCLASS()
class CHIMERAGAS_API UChimeraGameplayAbility :
    public UGameplayAbility
{
    GENERATED_BODY()

    friend class UChimeraAbilitySystemComponent;

    //----- Overrides -----//
public: 
    UChimeraGameplayAbility();

    // UGameplayAbility Begin
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override final;
    // UGameplayAbility End

    //----- Native Events -----//
protected:

    /** Cleanup logic that will run before ability has ended, meaning tags and tasks have not been cleaned up yet. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void PreEndAbility(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo& ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
    virtual void PreEndAbility_Implementation(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo& ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) {}

    /** Cleanup logic that will run after ability has ended, meaning tags and tasks have been cleaned up yet. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void PostEndAbility(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo& ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
    virtual void PostEndAbility_Implementation(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo& ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) {}

    //----- Gameplay Effects -----//
protected:

    UFUNCTION(BlueprintCallable)
    virtual FGameplayEffectSpecHandle MakeOutgoingSpecFromDef(const FGameplayEffectSpecDef& SpecDef) const;

    //----- Utility -----//
protected:

    template<class T>
    T* GetAvatar() const
    {
        return Cast<T>(GetAvatarActorFromActorInfo());
    }

    //----- Class Properties -----//
public:

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    EAbilityActivationPolicy ActivationPolicy{ EAbilityActivationPolicy::OnInput };

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGASInputEvent ActivationEvent;

    //----- Instance Variables -----//
protected:

};

