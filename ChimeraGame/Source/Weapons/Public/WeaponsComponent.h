// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "WeaponsComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, config = Game, meta = (BlueprintSpawnableComponent))
class WEAPONS_API UWeaponsComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UWeaponsComponent();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

public:

	UFUNCTION(BlueprintCallable)
	TArray<class UStaticMeshComponent*> GetWeaponMeshes(FGameplayTagContainer WeaponSlots) const;

	UFUNCTION(BlueprintCallable, meta = (Categories = "Weapons.Slot"))
	void ActivateWeapon(FGameplayTagContainer WeaponSlots);

	UFUNCTION(BlueprintCallable, meta = (Categories = "Weapons.Slot"))
	void DeactivateWeapon(FGameplayTagContainer WeaponSlots);

protected:

	void CollectWeaponMeshes();

	UFUNCTION(BlueprintNativeEvent)
	void HandleWeaponOverlap(
		UPrimitiveComponent* OverlappedComp, 
		AActor* Other, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, config)
	FName WeaponMeshComponentTag{ "WeaponMesh" };

	//----- Instance Variables -----//
protected:

	UPROPERTY(VisibleAnywhere)
	FGameplayTagContainer ActiveWeaponMeshes;

	UPROPERTY(VisibleAnywhere, meta = (ForceInlineRow, Categories = "Weapons.Slot"))
	TMap<FGameplayTag, TWeakObjectPtr<class UStaticMeshComponent>> WeaponMeshesBySlot;

	TWeakObjectPtr<class UChimeraAbilitySystemComponent> AbilitySystemComponent;
};