// Copyright Alec Greene. All Rights Reserved.

#include "WeaponsComponent.h"

#include "Components/StaticMeshComponent.h"

#include "ChimeraAbilitySystemComponent.h"
#include "ChimeraGASFunctionLibrary.h"
#include "WeaponData.h"
#include "WeaponsTags.h"

UWeaponsComponent::UWeaponsComponent()
{
}

#if WITH_EDITOR
void UWeaponsComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	CollectWeaponMeshes();
}
#endif

void UWeaponsComponent::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent = UChimeraGASFunctionLibrary::GetChimeraASC(GetOwner());

	CollectWeaponMeshes();
}

void UWeaponsComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
}

UWeaponsComponent* UWeaponsComponent::GetWeaponsComponent(AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UWeaponsComponent>() : nullptr;
}

TArray<UStaticMeshComponent*> UWeaponsComponent::GetWeaponMeshes(FGameplayTagContainer WeaponSlots) const
{
	TArray<UStaticMeshComponent*> Output;

	for (FGameplayTag SlotTag : WeaponSlots)
	{
		const TWeakObjectPtr<UStaticMeshComponent>* MeshComponentPtr = WeaponMeshesBySlot.Find(SlotTag);
		if (MeshComponentPtr && MeshComponentPtr->IsValid())
		{
			Output.Add(MeshComponentPtr->Get());
		}
	}

	return Output;
}

void UWeaponsComponent::ActivateWeapons(FGameplayTagContainer WeaponSlots)
{
	for (FGameplayTag SlotTag : WeaponSlots)
	{
		if (!ActiveWeaponMeshes.HasTagExact(SlotTag))
		{
			const TWeakObjectPtr<UStaticMeshComponent>* MeshComponentPtr = WeaponMeshesBySlot.Find(SlotTag);
			if (MeshComponentPtr && MeshComponentPtr->IsValid())
			{
				UStaticMeshComponent* MeshComponent = MeshComponentPtr->Get();
				MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleWeaponOverlap);

				ActiveWeaponMeshes.AddTag(SlotTag);
			}
		}
	}
}

void UWeaponsComponent::DeactivateWeapons(FGameplayTagContainer WeaponSlots)
{
	for (FGameplayTag SlotTag : WeaponSlots)
	{
		if (ActiveWeaponMeshes.HasTagExact(SlotTag))
		{
			const TWeakObjectPtr<UStaticMeshComponent>* MeshComponentPtr = WeaponMeshesBySlot.Find(SlotTag);
			if (MeshComponentPtr && MeshComponentPtr->IsValid())
			{
				UStaticMeshComponent* MeshComponent = MeshComponentPtr->Get();
				MeshComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::HandleWeaponOverlap);

				ActiveWeaponMeshes.RemoveTag(SlotTag);
			}
		}
	}
}

const UWeaponData* UWeaponsComponent::GetWeaponData(FGameplayTag WeaponSlot) const
{
	const TObjectPtr<const UWeaponData>* DataPtr = WeaponsBySlot.Find(WeaponSlot);
	return DataPtr ? DataPtr->Get() : nullptr;
}

void UWeaponsComponent::GiveWeapon(FGameplayTag WeaponSlot, const UWeaponData* WeaponData)
{
	TObjectPtr<const UWeaponData>& SlotDataPtr = WeaponsBySlot.FindOrAdd(WeaponSlot);
	SlotDataPtr = WeaponData;
}

bool UWeaponsComponent::EquipWeapon(FGameplayTag WeaponSlot, const UWeaponData* WeaponData)
{
	if (TObjectPtr<const UWeaponData>* SlotDataPtr = WeaponsBySlot.Find(WeaponSlot))
	{
		*SlotDataPtr = WeaponData;
		return true;
	}

	return false;
}

void UWeaponsComponent::CollectWeaponMeshes()
{
	const AActor* Owner = GetOwner();
	if (Owner == nullptr)
	{
		return;
	}

	for (UActorComponent* OwnerComponent 
		: Owner->GetComponentsByTag(UStaticMeshComponent::StaticClass(), WeaponMeshComponentTag))
	{
		if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(OwnerComponent))
		{
			for (FName NameTag : StaticMeshComponent->ComponentTags)
			{
				FGameplayTag GameplayTag = FGameplayTag::RequestGameplayTag(NameTag, false);
				if (GameplayTag.IsValid() && GameplayTag.MatchesTag(WeaponsTags::Slot))
				{
					WeaponMeshesBySlot.Add(GameplayTag, StaticMeshComponent);
					break;
				}
			}
		}
	}
}

void UWeaponsComponent::HandleWeaponOverlap_Implementation(
	UPrimitiveComponent* OverlappedComp,
	AActor* Other,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!AbilitySystemComponent.IsValid() || Other == GetOwner())
	{
		return;
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwner();
	EventData.Target = Other;
	EventData.OptionalObject = OverlappedComp;
	EventData.OptionalObject2 = OtherComp;

	// Since this event is going to the owner, who already knows their tags, we use instigator tags for context tags.
	if (const FGameplayTag* SlotTag = WeaponMeshesBySlot.FindKey(Cast<UStaticMeshComponent>(OverlappedComp)))
	{
		EventData.InstigatorTags.AddTagFast(*SlotTag);
	}

	AbilitySystemComponent->HandleGameplayEvent(WeaponsTags::Event_Hit, &EventData);
}
