// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "WeaponsComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, config = Game, meta = (BlueprintSpawnableComponent))
class WEAPONS_API UWeaponsComponent : public UActorComponent
{
	GENERATED_BODY()

	//----- Overrides -----//
public:

	UWeaponsComponent();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	//----- Feature Methods -----//
public:

	UFUNCTION(BlueprintCallable)
	static UWeaponsComponent* GetWeaponsComponent(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	TArray<class UStaticMeshComponent*> GetWeaponMeshes(FGameplayTagContainer WeaponSlots) const;

	UFUNCTION(BlueprintCallable)
	void ActivateWeapons(UPARAM(meta = (Categories = "Weapons.Slot")) FGameplayTagContainer WeaponSlots);

	UFUNCTION(BlueprintCallable)
	void DeactivateWeapons(UPARAM(meta = (Categories = "Weapons.Slot")) FGameplayTagContainer WeaponSlots);

	UFUNCTION(BlueprintCallable)
	const class UWeaponData* GetWeaponData(UPARAM(meta = (Categories = "Weapons.Slot")) FGameplayTag WeaponSlot) const;

	//----- Utility Methods -----//
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
	
	//----- Class Settings -----//
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, config)
	FName WeaponMeshComponentTag{ "WeaponMesh" };

	//----- Instance Variables -----//
protected:

	// agreene 2023/12/5 - #ToDo #Weapons Should this be a FGameplayTagCountContainer?
	UPROPERTY(VisibleAnywhere)
	FGameplayTagContainer ActiveWeaponMeshes;

	/** Weapon data in each slot representing the gamelpay properties of the weapon. */
	UPROPERTY(VisibleAnywhere, meta = (ForceInlineRow, Categories = "Weapons.Slot"))
	TMap<FGameplayTag, TObjectPtr<const class UWeaponData>> WeaponsBySlot;

	/** References to weapon meshes on the owning actor, used to visualize weapons. */
	UPROPERTY(VisibleAnywhere, meta = (ForceInlineRow, Categories = "Weapons.Slot"))
	TMap<FGameplayTag, TWeakObjectPtr<class UStaticMeshComponent>> WeaponMeshesBySlot;

	TWeakObjectPtr<class UChimeraAbilitySystemComponent> AbilitySystemComponent;
};