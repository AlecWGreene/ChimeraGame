// Copyright Alec Greene. All Rights Reserved.

#include "WeaponsComponent.h"

#include "Components/StaticMeshComponent.h"

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

	CollectWeaponMeshes();
}

void UWeaponsComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
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
