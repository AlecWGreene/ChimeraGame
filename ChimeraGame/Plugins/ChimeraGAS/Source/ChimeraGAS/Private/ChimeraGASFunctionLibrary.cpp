#include "ChimeraGASFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "ChimeraAbilitySystemComponent.h"

UChimeraAbilitySystemComponent* UChimeraGASFunctionLibrary::GetASCFromActor(AActor* Actor)
{
	return GetASCFromActor<UChimeraAbilitySystemComponent>(Actor);
}