// Copyright Alec Greene. All Rights Reserved.
#include "ChimeraAttributeSet.h"

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
					AttributeDefaults.Add(Attribute, 0.f);
				}
			}
		}
	}
}
#endif
