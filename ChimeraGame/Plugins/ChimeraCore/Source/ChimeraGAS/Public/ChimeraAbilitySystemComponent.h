#pragma once

#include "AbilitySystemComponent.h"
#include "InputAction.h"

#include "ChimeraAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGASInputEventDelegate, const FInputActionInstance&, InputActionInstance);

USTRUCT(BlueprintType)
struct CHIMERAGAS_API FGASInputEvent
{
    GENERATED_BODY()

    FGASInputEvent() {}

    FGASInputEvent(const UInputAction* InInputAction, ETriggerEvent InTriggerEvent = ETriggerEvent::Triggered) 
    {
        InputAction = InInputAction;
        TriggerEvent = InTriggerEvent;
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<const UInputAction> InputAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETriggerEvent TriggerEvent{ ETriggerEvent::Triggered };

    bool IsValid() const
    {
        return InputAction && TriggerEvent != ETriggerEvent::None;
    }

    bool operator == (const FGASInputEvent& Other) const
    {
        return InputAction == Other.InputAction && TriggerEvent == Other.TriggerEvent;
    }
};

FORCEINLINE uint32 GetTypeHash(const FGASInputEvent& Event)
{
    const uint32 Hash = HashCombine(GetTypeHash(Event.InputAction), GetTypeHash(Event.TriggerEvent));
    return Hash;
}

UCLASS()
class CHIMERAGAS_API UChimeraAbilitySystemComponent :
    public UAbilitySystemComponent
{
    GENERATED_BODY()

    //----- Overrides -----//
public:
    UChimeraAbilitySystemComponent();

    virtual void BindToInputComponent(UInputComponent* InputComponent) override;
    virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
    virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;
    virtual int32 HandleGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) override;

    //----- Input -----//
public:
    virtual void BindAbilityInput(const FGameplayAbilitySpec& Spec);
    virtual void UnbindAbilityInput(const FGameplayAbilitySpec& Spec);
    virtual FGASInputEventDelegate& FindOrAddGASInputEventDelegate(const FGASInputEvent& InputEvent);

protected:
    virtual void HandleInputEvent(const FInputActionInstance& InputActionInstance);

    //----- Attributes -----//
public:

    virtual const UAttributeSet* ApplyInitializer(const class UChimeraAttributeSetInitializer* Initializer);

    //----- Getters -----//
public:

    template<class T>
    T* GetAvatar() const
    {
        return Cast<T>(GetAvatarActor())
    }

    //----- Class Properties -----//
protected:

    //----- Instance Variables -----//
public:
    TArray<FGameplayAbilitySpecHandle> InputPressedHandles;
    TArray<FGameplayAbilitySpecHandle> InputHeldHandles;
    TArray<FGameplayAbilitySpecHandle> InputReleasedHandles;

    TMap<FGASInputEvent, TSet<FGameplayAbilitySpecHandle>> AbilityInputActivations;
    TMap<FGASInputEvent, FGASInputEventDelegate> GASInputEventDelegates;

    TWeakObjectPtr<class UEnhancedInputComponent> CachedInputComponent;
};

