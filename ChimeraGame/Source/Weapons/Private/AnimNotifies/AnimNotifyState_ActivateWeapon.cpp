// Copyright Alec Greene. All Rights Reserved.
#include "AnimNotifies/AnimNotifyState_ActivateWeapon.h"

#include "WeaponsComponent.h"

void UAnimNotifyState_ActivateWeapon::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		WeaponsComponent = UWeaponsComponent::GetWeaponsComponent(MeshComp->GetOwner());
		if (WeaponsComponent.IsValid())
		{
			WeaponsComponent->ActivateWeapons(WeaponSlots);
		}
	}
}

void UAnimNotifyState_ActivateWeapon::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (WeaponsComponent.IsValid())
	{
		WeaponsComponent->DeactivateWeapons(WeaponSlots);
	}
}
