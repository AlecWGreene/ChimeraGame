#include "ChimeraGASFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "ChimeraAbilitySystemComponent.h"

UChimeraAbilitySystemComponent* UChimeraGASFunctionLibrary::GetChimeraASC(const AActor* Actor)
{
	return GetASCFromActor<UChimeraAbilitySystemComponent>(Actor);
}