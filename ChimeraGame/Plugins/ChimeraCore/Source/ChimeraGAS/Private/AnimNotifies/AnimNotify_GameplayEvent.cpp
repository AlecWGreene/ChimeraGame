// Copyright Alec Greene. All Rights Reserved.
#include "AnimNotifies/AnimNotify_GameplayEvent.h"

#include "AbilitySystemComponent.h"

void UAnimNotify_GameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner()))
	{
		ASC->HandleGameplayEvent(EventTag, &EventData);
	}
}