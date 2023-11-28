#pragma once

#include "Abilities/GameplayAbility.h"

#include "ChimeraGameplayAbility.generated.h"

UENUM()
enum class EAbilityActivationPolicy : uint8
{
    /** Activate when GiveAbility is called. */
    OnGranted,
    /** Wait for an ability trigger to activate. */
    OnAbilityTrigger,
    /** Activate when the input is pressed. */
    OnInputPressed,
    /** Attempt to active each frame as long as the input is held. */
    WhileInputHeld
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
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) final;
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
    EAbilityActivationPolicy ActivationPolicy{ EAbilityActivationPolicy::OnInputPressed };

    UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (Categories = "Input", EditConditionHides, EditCondition="ActivationPolicy==EAbilityActivationPolicy::OnInputPressed||ActivationPolicy==EAbilityActivationPolicy::WhileInputHeld"))
    FGameplayTag InputTag;

    //----- Instance Variables -----//
protected:

};

