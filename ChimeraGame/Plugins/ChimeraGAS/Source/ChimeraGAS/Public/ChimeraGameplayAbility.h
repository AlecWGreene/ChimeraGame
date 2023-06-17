#pragma once

#include "Abilities/GameplayAbility.h"

#include "ChimeraGameplayAbility.generated.h"

UCLASS()
class CHIMERAGAS_API UChimeraGameplayAbility :
    public UGameplayAbility
{
    GENERATED_BODY()

    //----- Constructor and Engine Events -----//
public:
    UChimeraGameplayAbility();

    //----- UGameplayAbility Overrides -----//
public:
    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;
    virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData = nullptr) override;
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
    virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
    virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
    virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
    virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility);
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

    //----- Instance Lifecycle -----//
public:

    /** */
    UFUNCTION(BlueprintNativeEvent)
    bool CanActivateInstance(const FGameplayTagContainer& SourceTags, const FGameplayTagContainer& TargetTags, FGameplayTagContainer& OptionalRelevantTags);

    /** Entry point for all ability logic. */
    UFUNCTION(BlueprintNativeEvent)
    void ActivateInstance();
    
    UFUNCTION(BlueprintNativeEvent)
    bool CheckInstanceCooldown(FGameplayTagContainer& OptionalRelevantTags) const;
    
    UFUNCTION(BlueprintNativeEvent)
    void ApplyInstanceCooldown() const;
    
    UFUNCTION(BlueprintNativeEvent)
    bool CheckInstanceCost(FGameplayTagContainer& OptionalRelevantTags) const;
    
    UFUNCTION(BlueprintNativeEvent)
    void ApplyInstanceCost() const;
    
    /** 
    * Hook for logic to run when the ability is cancelled through CancelAbility. Essentially functions as a wrapper for any logic 
    * which would go in a if(bWasCancelled) scope within EndInstance.
    */
    UFUNCTION(BlueprintNativeEvent)
    void CancelInstance();
    
    /** 
    * EndAbility logic which runs on instances before the ability tags/tasks have been cleaned up.
    * 
    * This is the optimal place to perform last ditch logic under the context of the ability.
    * 
    * @param bWasCancelled True if the ability was cancelled instead of running its course.
    */
    UFUNCTION(BlueprintNativeEvent)
    void EndInstance(bool bWasCancelled);

    /**
    * EndAbility logic which runs on instances after the ability tags/tasks have been cleaned up.
    *
    * This is the optimal place to perform GAS actions which want to be chained onto this ability,
    * but may be blocked by this ability being active (such as activating another ability).
    *
    * @param bWasCancelled True if the ability was cancelled instead of running its course.
    */
    UFUNCTION(BlueprintNativeEvent)
    void PostEndInstance(bool bWasCancelled);

    //----- Class Properties -----//
protected:

    //----- Instance Variables -----//
protected:

    /** Cached event data to allow access outside of the scope of ActivateAbility. */
    FGameplayEventData CachedEventData;
};

