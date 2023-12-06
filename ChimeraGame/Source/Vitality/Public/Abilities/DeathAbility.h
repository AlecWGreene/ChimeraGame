// Copyright Alec Greene. All Rights Reserved.
#include "Abilities/ChimeraGameplayAbility.h"

#include "DeathAbility.generated.h"

UCLASS(Blueprintable)
class VITALITY_API UDeathAbility : public UChimeraGameplayAbility
{
	GENERATED_BODY()

	//----- Overrides -----//
public:

	UDeathAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void PreEndAbility_Implementation(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo& ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void PostEndAbility_Implementation(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo& ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//----- Utility Methods -----//
public:

	UFUNCTION()
	void HandleMontageComplete();

	//----- Class Settings -----//
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "AnimSet"))
	FGameplayTag DeathAnimSetTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Anim.Montage.Death"))
	FGameplayTag DeathMontageTag;
};