// Copyright Alec Greene. All Rights Reserved.

#include "WeaponsComponent.h"

#include "Components/ShapeComponent.h"
#include "Components/StaticMeshComponent.h"

#include "ChimeraAbilitySystemComponent.h"
#include "ChimeraGASFunctionLibrary.h"
#include "WeaponData.h"
#include "WeaponsTags.h"

TAutoConsoleVariable<bool> CVar_Weapons_ShowCollision(TEXT("Weapons.Collision.Show"), false, TEXT(""));

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

void UWeaponsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunction)
{
	Super::TickComponent(DeltaTime, TickType, TickFunction);
}

void UWeaponsComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
}

UWeaponsComponent* UWeaponsComponent::GetWeaponsComponent(AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UWeaponsComponent>() : nullptr;
}

TArray<UPrimitiveComponent*> UWeaponsComponent::GetWeaponMeshes(FGameplayTagContainer WeaponSlots) const
{
	TArray<UPrimitiveComponent*> Output;

	for (FGameplayTag SlotTag : WeaponSlots)
	{
		const TWeakObjectPtr<UPrimitiveComponent>* CollisionComponentPtr = CollisionComponentsBySlot.Find(SlotTag);
		if (CollisionComponentPtr && CollisionComponentPtr->IsValid())
		{
			Output.Add(CollisionComponentPtr->Get());
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
			const TWeakObjectPtr<UPrimitiveComponent>* CollisionComponentPtr = CollisionComponentsBySlot.Find(SlotTag);
			if (CollisionComponentPtr && CollisionComponentPtr->IsValid())
			{
				UPrimitiveComponent* CollisionComponent = CollisionComponentPtr->Get();
				CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleWeaponOverlap);

				if (CVar_Weapons_ShowCollision.GetValueOnGameThread() && CollisionComponent->IsA<UShapeComponent>())
				{
					CollisionComponent->SetVisibility(true);
					CollisionComponent->SetHiddenInGame(false);
				}

				ActiveWeaponMeshes.AddTag(SlotTag);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to find collision component for slot %s"), *SlotTag.ToString());
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
			const TWeakObjectPtr<UPrimitiveComponent>* CollisionComponentPtr = CollisionComponentsBySlot.Find(SlotTag);
			if (CollisionComponentPtr && CollisionComponentPtr->IsValid())
			{
				UPrimitiveComponent* CollisionComponent = CollisionComponentPtr->Get();
				CollisionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::HandleWeaponOverlap);

				if (CVar_Weapons_ShowCollision.GetValueOnGameThread() && CollisionComponent->IsA<UShapeComponent>())
				{
					CollisionComponent->SetVisibility(false);
					CollisionComponent->SetHiddenInGame(true);
				}

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
		: Owner->GetComponentsByTag(UPrimitiveComponent::StaticClass(), WeaponMeshComponentTag))
	{
		if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(OwnerComponent))
		{
			for (FName NameTag : PrimitiveComponent->ComponentTags)
			{
				FGameplayTag GameplayTag = FGameplayTag::RequestGameplayTag(NameTag, false);
				if (GameplayTag.IsValid() && GameplayTag.MatchesTag(WeaponsTags::Slot))
				{
					CollisionComponentsBySlot.Add(GameplayTag, PrimitiveComponent);
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
	if (const FGameplayTag* SlotTag = CollisionComponentsBySlot.FindKey(Cast<UStaticMeshComponent>(OverlappedComp)))
	{
		EventData.InstigatorTags.AddTagFast(*SlotTag);
	}

	AbilitySystemComponent->HandleGameplayEvent(WeaponsTags::Event_Hit, &EventData);
}
