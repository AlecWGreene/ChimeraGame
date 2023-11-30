// Copyright Alec Greene. All Rights Reserved.
#include "ChimeraAttributeSet.h"

DEFINE_LOG_CATEGORY_STATIC(LogChimeraAttributeSet, Log, All);

UChimeraAttributeSetInitializer::UChimeraAttributeSetInitializer()
{
	
}

#if WITH_EDITOR
void UChimeraAttributeSetInitializer::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UChimeraAttributeSetInitializer, AttributeSetClass))
	{
		AttributeDefaults.Reset();
		if (IsValid(AttributeSetClass))
		{
			for (TFieldIterator<FProperty> It(AttributeSetClass); It; ++It)
			{
				FStructProperty* StructProperty = CastField<FStructProperty>(*It);
				if (StructProperty && 
					StructProperty->Struct && 
					StructProperty->Struct->IsChildOf(FGameplayAttributeData::StaticStruct()))
				{
					// Do not provide defaults for meta attributes since they will get cleared on modification anyways,
					// or if we explicitly say we don't want an initializer for it
					if (StructProperty->HasMetaData("MetaAttribute") || StructProperty->HasMetaData("DoNotInitialize"))
					{
						continue;
					}

					FGameplayAttribute Attribute = FGameplayAttribute(StructProperty);
					AttributeDefaults.Add(Attribute.AttributeName, 0.f);
				}
			}
		}
	}
}
#endif


bool UChimeraAttributeSetInitializer::InitializeAttributeSet(UAttributeSet* TargetSet) const
{
	if (IsValid(TargetSet) && TargetSet->StaticClass() == AttributeSetClass)
	{
		for (TPair<FString, float> InitialData : AttributeDefaults)
		{
			if (FProperty* Attribute = AttributeSetClass->FindPropertyByName(FName(InitialData.Key)))
			{
				FGameplayAttributeData* AttributeData = Attribute->ContainerPtrToValuePtr<FGameplayAttributeData>(TargetSet);
				if (ensureAlwaysMsgf(AttributeData, TEXT("Failed to read attribute data for %s in %s"), 
					*InitialData.Key, *GetNameSafe(TargetSet)))
				{
					AttributeData->SetBaseValue(InitialData.Value);
				}
			}
			else
			{
				UE_LOG(LogChimeraAttributeSet, Warning, TEXT("Failed to find attribute %s in set %s"), 
					*InitialData.Key, *GetNameSafe(AttributeSetClass));
			}
		}

		return true;
	}

	return false;
}